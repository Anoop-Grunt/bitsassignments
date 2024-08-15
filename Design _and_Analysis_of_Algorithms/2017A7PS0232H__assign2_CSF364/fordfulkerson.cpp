#include <iostream>
#include <limits.h>
#include <fstream>
#include <queue>
#include <string.h>
#include <stdio.h>

using namespace std;

#define V 200
int no_of_vertices,no_of_edges; ///test variables for q1
int no_of_vertices_1,no_of_vertices_2,temp; ///test variables for q3
#define INPUT  "small_g3.txt"	 ///Input
#define OUTPUT "Output_small_g3.txt"  ///Output

/// function prototypes
void max_flow(int graph[V][V], int source, int sink,int method);   ///Gives Maxflow of a given graph
bool aug_path(int residual[V][V], int source, int sink, int parent[]);  ///Returns if a graph has any augment paths left
void mincut_finder(int residual[V][V], int source, bool visited[]);    ///Finding the mincut
void print_mincut(int graph[V][V], bool visited[], int source, int sink);  ///Printing mincut edges
			    
int main(){
    int method;
    cout << "enter 1 for fordfulkerson with min-cut, 2 for without min-cut and 3 for max bipartite problem" << endl;
    cin >> method;   

    if(method == 1 || method == 2){

        freopen(OUTPUT, "w", stdout);
        ifstream cin(INPUT, ifstream::in);
        
        int graph[V][V] = {0};
        
        int source, sink, vertex1, vertex2, weight;
        if (cin.is_open()){
            cin >> no_of_vertices >> no_of_edges;
            cin >> source >> sink;
            ///first line has source and sink of graph
            while(!cin.eof()){
                cin >> vertex1; cin >> vertex2; cin >> weight;
                graph[vertex1][vertex2] = weight;
            }
            /// inserting weights to in graph matrix
        }

        max_flow(graph, source, sink, method);
     
    }
    else if (method == 3){
        
        freopen(OUTPUT, "w", stdout);
        ifstream cin(INPUT, ifstream::in);
        
        int graph[V][V] = {0};
        
        int source, sink, vertex1, vertex2, weight;
        if (cin.is_open()){
            cin >> no_of_vertices_1 >> no_of_vertices_2 >> temp;
            source = 0;
            sink = no_of_vertices_1 + no_of_vertices_2 + 1 ;
            while(!cin.eof()){
                cin >> vertex1; cin >> vertex2;
                graph[vertex1][no_of_vertices_1 + vertex2] = 1;
                graph[source][vertex1] = 1;
                graph[no_of_vertices_1 + vertex2][sink] = 1;
            }
            /// inserting bool values to in graph matrix
        }
        
        max_flow(graph, source, sink, method);
        
    }
    else{}
    

    return 0;
}

///Gives Maxflow of a given graph
void max_flow(int graph[V][V], int source, int sink, int method){
    int u, v, it = 0;    
    int residual[V][V];  
    for (u = 0; u < V; u++){
        for (v = 0; v < V; v++){
            residual[u][v] = graph[u][v];
        }
    }    
    /// creation of a residual graph
    int path[V]; 
    /// this path array brings the augment path(if there exists one) from the function aug_path
    int max_flow = 0;  
    ///initialzing with no flow
    while (aug_path(residual, source, sink, path)){
        
        int path_flow = INT_MAX;
        for (v = sink; v != source; v = path[v]){
            u = path[v];
            path_flow = min(path_flow, residual[u][v]);  ///Bottlenecking
        }
        ///Updating residual capacities
        for (v = sink; v != source; v = path[v]){
	        u = path[v];
            residual[u][v] -= path_flow;
            residual[v][u] += path_flow;
        }
        /// max_flow is added by bottleneck
        max_flow += path_flow;
        
    }
    
    /// finding vertices reachable from source after flow is maximised
    bool tempvisited[V];
    memset(tempvisited, false, sizeof(tempvisited));
    
    if(method == 1){
        mincut_finder(residual, source, tempvisited);
        print_mincut(graph, tempvisited, source, sink);
    }
    cout << "Max-flow value : " << max_flow << endl << endl;

}

///Stores augment path in path[] if path exists
bool aug_path(int residual[V][V], int source, int sink, int path[]){
    bool visited[V];       
    memset(visited, 0, sizeof(visited));
 
    queue <int> Q;         
    Q.push(source);     ///Queue for maintaining nodes to be pushed       
    visited[source] = true;     
    path[source] = -1;
    ///BFS
    while (Q.empty()==false){
        int some_vertex = Q.front();
        Q.pop();
 
        for (int v = 0; v < V; v++){
            if (visited[v] == false && residual[some_vertex][v] > 0){
                Q.push(v);
                path[v] = some_vertex;
                visited[v] = true;
            }
        }
    }
    ///If sink is visited then path exists
    return (visited[sink] == true);
}

/// DFS to find all vertex reachable from Source vertex.
void mincut_finder(int residual[V][V], int source, bool visited[]){
    visited[source] = true;
    for (int i = 0; i < V; i++){
        if (residual[source][i] && !visited[i]){
            mincut_finder(residual, i, visited);
        }
    }
}

/// Print function for mincut capacity and severing edges 
void print_mincut(int graph[V][V], bool visited[], int source, int sink){
    int mincut_size = 0, mincut_cap = 0;
    cout << "Min st-cut:" <<endl;
    for (int i = 0; i < V; i++){
        for (int j = 0; j < V; j++){
            if (visited[i] && !visited[j] && graph[i][j]){    
                cout << i << " - " << j << endl;
                mincut_size++;
                mincut_cap += graph[i][j];
            }
        }
    }    
    cout << endl;
    
    cout << "Min-cut capacity: " << mincut_cap << endl << endl;
}