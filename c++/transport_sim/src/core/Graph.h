#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
struct Node {
  int id;
  double lat, lon;
};

struct Edge {
  int id, from, to;
  double length_m;
  double speed_kmh;
  double max_tonnage_t;
};

class Graph {
    private:
      std::vector<Node> nodes;
      std::vector<Edge> edges;

    public:
      void addNode(int id, double lat, double lon);
      void addEdge(int id, int from, int to, double length_m, double speed_kmh, double max_tonnage_t);
      const std::vector<Node>& getNodes() const;
      const std::vector<Edge>& getEdges() const;
};



#endif //GRAPH_H
