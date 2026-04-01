#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if(!(cin >> n >> m)) return 0;
    vector<vector<int>> adj(n+1);
    for(int i=0;i<m;i++){
        int x,y;cin>>x>>y;
        if(x==y) continue;
        if(x<1||x>n||y<1||y>n) continue;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    vector<int> dfn(n+1,0), low(n+1,0), parent(n+1,0), it(n+1,0);
    int timer=0;
    vector<pair<int,int>> estack; estack.reserve((size_t)max(1, m));
    vector<char> on_odd_cycle(n+1, 0);
    vector<int> seen(n+1, 0); int mark_id = 1;

    for(int s=1; s<=n; ++s){
        if(dfn[s]) continue;
        // iterative DFS starting at s
        vector<int> st; st.reserve(1024);
        parent[s]=0; dfn[s]=low[s]=++timer; it[s]=0; st.push_back(s);
        while(!st.empty()){
            int u = st.back();
            if(it[u] < (int)adj[u].size()){
                int v = adj[u][it[u]++];
                if(v == parent[u]) continue;
                if(!dfn[v]){
                    parent[v]=u; dfn[v]=low[v]=++timer; it[v]=0;
                    estack.emplace_back(u,v);
                    st.push_back(v);
                } else if(dfn[v] < dfn[u]){
                    // back edge
                    estack.emplace_back(u,v);
                    low[u] = min(low[u], dfn[v]);
                }
            } else {
                st.pop_back();
                int p = parent[u];
                if(p){
                    low[p] = min(low[p], low[u]);
                    if(low[u] >= dfn[p]){
                        // Pop one biconnected component ending with (p,u)
                        vector<pair<int,int>> comp_edges;
                        comp_edges.reserve(4);
                        while(true){
                            auto e = estack.back(); estack.pop_back();
                            comp_edges.push_back(e);
                            if((e.first==p && e.second==u) || (e.first==u && e.second==p)) break;
                        }
                        // Collect unique nodes and test bipartiteness inside this block
                        mark_id++;
                        vector<int> nodes; nodes.reserve(comp_edges.size()*2);
                        for(auto &e: comp_edges){
                            if(seen[e.first] != mark_id){ seen[e.first] = mark_id; nodes.push_back(e.first);} 
                            if(seen[e.second] != mark_id){ seen[e.second] = mark_id; nodes.push_back(e.second);} 
                        }
                        int k = (int)nodes.size();
                        if(k >= 3){
                            unordered_map<int,int> idx; idx.reserve(k*2+1);
                            for(int i=0;i<k;i++) idx[nodes[i]] = i;
                            vector<vector<int>> ladj(k);
                            for(auto &e: comp_edges){
                                int a = idx[e.first];
                                int b = idx[e.second];
                                if(a==b) continue;
                                ladj[a].push_back(b);
                                ladj[b].push_back(a);
                            }
                            vector<int> color(k, -1);
                            bool non_bip = false;
                            for(int i=0;i<k && !non_bip;i++) if(color[i]==-1){
                                queue<int> q; q.push(i); color[i]=0;
                                while(!q.empty() && !non_bip){
                                    int x = q.front(); q.pop();
                                    for(int y: ladj[x]){
                                        if(color[y]==-1){ color[y]=color[x]^1; q.push(y); }
                                        else if(color[y]==color[x]){ non_bip = true; break; }
                                    }
                                }
                            }
                            if(non_bip){
                                for(int vtx: nodes) on_odd_cycle[vtx] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    long long not_invitable = 0;
    for(int i=1;i<=n;i++) if(!on_odd_cycle[i]) not_invitable++;
    cout << not_invitable << '\n';
    return 0;
}
