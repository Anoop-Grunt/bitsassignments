#include "Graph.h"

Graph :: Graph(int V, int E) {
    this->V = V;
    this->E = E;
    adjaceny_list = new vector<int>[V];
    for(int i = 0; i < V; ++i) {
        vertices.push_back(i);
    }
}

void Graph :: print_graph() { //Printing graph with the child of every index in adjacency list
    for(int i = 0; i < V; ++i) {
        cout << i << " ";
        for(int child : adjaceny_list[i]) {
            cout << child << " ";
        }
        cout << endl;
    }
}

void Graph :: dfs(int src, vector<bool> &visited) { //Depth First Search
    visited[src] = true;
    cout << src << " ";
    for(int child : adjaceny_list[src]) {
        if(!visited[child])
          dfs(child, visited);
    }
}

void Graph :: finishing_time(int src, vector<bool> &visited, stack<int> &order) { //pushing vertices to stack based on initial steps
    visited[src] = true;

    for(int child : adjaceny_list[src]) {
        if(!visited[child])
            finishing_time(child, visited, order);
    }

    order.push(src);
}

void Graph :: add_edge(int u, int v) { //adding an edge depicted in input
    adjaceny_list[u].push_back(v);
}

Graph Graph :: get_transpose() { //Transposing the graph
    Graph g_transpose(V, E);
    for(int u = 0; u < V; ++u) {
        for(int v : adjaceny_list[u]) {
            g_transpose.adjaceny_list[v].push_back(u);
        }
    }
    return g_transpose; //returning the transpose of a graph
}

void Graph :: print() { //main calls this function //this function uses all other functions to get the result
    vector<bool> visited(V, false);
    stack<int> order;

    for(int vertex = 0; vertex < V; ++vertex) {
        if(!visited[vertex])
          finishing_time(vertex, visited, order);
    }

    Graph g_transpose = get_transpose();

    for(int i = 0; i < V; ++i) visited[i] = false;

    while(!order.empty()) {
        int vertex = order.top();
        order.pop();

        if(!visited[vertex]) {
            g_transpose.dfs(vertex, visited);
            cout << endl;
        }
    }
}