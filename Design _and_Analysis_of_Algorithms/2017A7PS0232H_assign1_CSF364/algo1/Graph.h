#ifndef GRAPH_H
#define GRAPH_H
#include <bits/stdc++.h>
using namespace std;

class Graph {
  public:
    int V;
    int E;
    vector<int> *adjaceny_list;
    vector<int> vertices;

    Graph(int V, int E);
    void add_edge(int u, int v);
    void print_graph();
    void dfs(int src, vector<bool> &visited);
    Graph get_transpose();
    void finishing_time(int src, vector<bool> &visited, stack<int> &order);

    void print();
};

#endif
