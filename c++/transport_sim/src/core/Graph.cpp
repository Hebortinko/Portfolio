#include "Graph.h"

int Graph::addNode(int id, double lat, double lon) {
    Node n{id, lat, lon};
    nodes.push_back(n);
    adjacency_list.emplace_back();
    return adjacency_list.size() - 1;
}

int Graph::addEdge(int id, int from, int to, double length_m, double speed_mps, double max_tonnage_t) {
    if (from < 0 || from >= (int)nodes.size())
        throw std::out_of_range("Graph::addEdge: from out of range");
    if (to < 0 || to >= (int)nodes.size())
        throw std::out_of_range("Graph::addEdge: to out of range");
    if (length_m <= 0.0)
        throw std::invalid_argument("Graph::addEdge: length_m must be > 0");
    if (speed_mps <= 0.0)
        throw std::invalid_argument("Graph::addEdge: speed_mps must be > 0");
    if (max_tonnage_t <= 0.0)
        throw std::invalid_argument("Graph::addEdge: max_tonnage_t must be > 0");

    Edge e{id, from, to, length_m, speed_mps, max_tonnage_t};
    edges.push_back(e);
    int edge_index = (int)edges.size() - 1;
    adjacency_list[from].push_back(edge_index);
    return edge_index;
}