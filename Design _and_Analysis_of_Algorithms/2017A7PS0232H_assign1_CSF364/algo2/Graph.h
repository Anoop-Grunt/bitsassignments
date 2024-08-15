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
    Graph get_transpose();
    void print();
    void DCSC(vector<int> vertices);

    vector<int> descendants(vector<int> *adj_list, int src, vector<int> vertices);
    vector<int> predecessors(int src, vector<int> vertices);
    
};

#endif
