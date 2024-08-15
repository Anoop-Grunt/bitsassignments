#include "Graph.h"
int main(int argc, char **argv) {
    string file_name;
    file_name = (argv[1]);
    //Input file name
    ifstream cin(file_name);
    int V, E; // V=vertice count, //E = edge count
    cin >> V >> E;
    Graph graph(V, E);
    //edge between two vertices of a directed graph 
    while(E--) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u, v); // adding an edge
    }
    clock_t tStart, tEnd;
    cout << "The strongly connected components " << endl;
    
    //Algorithm
    cout << "Standard Algorithm :- " << endl;
    //calculating start and end time
    tStart = clock();
    graph.print(); // This print function will call all necessary functions necessary
    tEnd = clock();
    
    cout << "End of Program" << endl;

    printf("Exeuction Time : %.5fs\n", (double)(tEnd - tStart)/CLOCKS_PER_SEC);
}
