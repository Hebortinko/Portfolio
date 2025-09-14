#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <stdexcept>
struct Node {
  int id;
  double lat;
  double lon;
};

struct Edge {
  int id;
  int from;
  int to;
  double length_m;
  double speed_mps;
  double max_tonnage_t;
};

class Graph {
    private:
      std::vector<Node> nodes;
      std::vector<Edge> edges;
      std::vector<std::vector<int>> adjacency_list;

    public:
      int addNode(int id, double lat, double lon);
      int addEdge(int id, int from, int to, double length_m, double speed_mps, double max_tonnage_t);

      const std::vector<Node>& getNodes() const { return nodes; }
      const std::vector<Edge>& getEdges() const { return edges; }
      const std::vector<int>& adjacency(int node_index) const { return adjacency_list.at(node_index); }

      int nodesCount() const { return static_cast<int>(nodes.size()); }
      int edgesCount() const { return static_cast<int>(edges.size()); }
};
#endif //GRAPH_H
