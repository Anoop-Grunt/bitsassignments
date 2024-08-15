#include "Graph.h"

Graph :: Graph(int V, int E) {
    this->V = V;
    this->E = E;
    adjaceny_list = new vector<int>[V];
    for(int i = 0; i < V; ++i) vertices.push_back(i);
}

void Graph :: add_edge(int u, int v) {
    //add edge from u to v
    adjaceny_list[u].push_back(v);
}

Graph Graph :: get_transpose() {
    Graph g_transpose(V, E);
    // for each u->v edge make v->u in the transpose
    for(int u = 0; u < V; ++u) {
        for(int v : adjaceny_list[u]) {
            g_transpose.adjaceny_list[v].push_back(u);
        }
    }
    return g_transpose;
}


vector<int> Graph :: descendants(vector<int> *adj_list, int src, vector<int> vertices) {
    //This function returns all the reachable vertices from src

    vector<int> desc_set;

    queue<int> q;
    q.push(src);
    vector<bool> visited(V, false);
    visited[src] = true;
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        desc_set.push_back(node);
        for(int child : adj_list[node]) {
            if(find(vertices.begin(), vertices.end(), child) == vertices.end()) continue;
            if(!visited[child]) {
                visited[child] = true;
                q.push(child);
            }
        }
    }

    return desc_set;
}

vector<int> Graph :: predecessors(int src, vector<int> vertices) {

    //This function returns all the vertices which can reach src
    //This can also be viewed as finding descendants of the transpose of the original graph

    Graph g_transpose = get_transpose();
    return descendants(g_transpose.adjaceny_list, src, vertices);
}

void Graph :: DCSC(vector<int> vertices) {

    if(vertices.size() == 0) return; //return if there are no vertices

    int ind = (rand() % vertices.size()); //randomly start by picking some vertex
    int pivot_vertex = vertices[ind];
    vector<int> desc = descendants(adjaceny_list, pivot_vertex, vertices);
    vector<int> pred = predecessors(pivot_vertex, vertices);
    vector<int> scc;
    sort(desc.begin(), desc.end());
    sort(pred.begin(), pred.end());
    sort(vertices.begin(), vertices.end());
    set_intersection(desc.begin(), desc.end(), pred.begin(), pred.end(), back_inserter(scc)); //intersection between the predecessors and descendants sets
    sort(scc.begin(), scc.end());

    for(int node : scc) cout << node << " ";
    cout << endl;
    
    vector<int> desc_minus_scc, pred_minus_scc, desc_union_pred, rem;

    set_difference(pred.begin(), pred.end(), scc.begin(), scc.end(), back_inserter(pred_minus_scc)); //Difference between predecessors and scc
    
    set_difference(desc.begin(), desc.end(), scc.begin(), scc.end(), back_inserter(desc_minus_scc));

    set_union(desc.begin(), desc.end(), pred.begin(), pred.end(), back_inserter(desc_union_pred));
    
    sort(desc_union_pred.begin(), desc_union_pred.end());

    set_difference(vertices.begin(), vertices.end(), desc_union_pred.begin(), desc_union_pred.end(), back_inserter(rem)); //rem = remaining set

    DCSC(pred_minus_scc);
    DCSC(desc_minus_scc);
    DCSC(rem);
}

void Graph :: print() {
    DCSC(vertices);
}