#include "Graph.h"
int main(int argc, char **argv) {
    string algo_number, string file_name;
    file_name = (argv[1]);
    ifstream cin(file_name);
    // V = number of vertices, E = number of Edges
    int V, E;
    cin >> V >> E;
    Graph graph(V, E);
    while(E--) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u, v); // Add an edge in the directed graph from the input file.
    }
    clock_t tStart, tEnd;
    cout << "The strongly connected components " << endl;
    cout << "Divide And Conquer Strong Components :- " << endl;
    //Calculating the start and end time by tStart and tEnd
    tStart = clock();
    graph.print(); //calling print function invokes all nested functions. This returns our desired result
    tEnd = clock();
    cout << "End of Program" << endl;

    printf("Exeuction Time : %.5fs\n", (double)(tEnd - tStart)/CLOCKS_PER_SEC);
}
