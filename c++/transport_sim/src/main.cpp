#include <iostream>
#include "core/Graph.h"
int main() {
    Graph graph;
    graph.addNode(0,48.15,17.10);
    graph.addNode(1,48.25,16.57);
    graph.addEdge(0,0,1,65000,130,40);

    std::cout << "Nodes: " << graph.getNodes().size() << " | Edges: " << graph.getEdges().size() << std::endl;
    return 0;
}
