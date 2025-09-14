#include "Graph.h"

void Graph::addNode(int id, double lat, double lon) {
    nodes.push_back({id, lat, lon});
};

void Graph::addEdge(int id, int from, int to, double length_m, double speed_kmh, double max_tonnage_t) {
    edges.push_back({id, from, to, length_m, speed_kmh, max_tonnage_t});
}

const std::vector<Node>& Graph::getNodes() const {
  return nodes;
}

const std::vector<Edge>& Graph::getEdges() const {
  return edges;
}