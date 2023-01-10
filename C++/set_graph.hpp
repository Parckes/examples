#include "igraph.hpp"
#include <set>

struct SetGraph: public IGraph {
public:
    SetGraph(size_t size): size(size) {};
    SetGraph(const IGraph* graph): size(graph->VerticesCount()) {
        for (size_t i = 0; i < size; ++i) {
            std::vector<size_t> adj;
            graph->GetNextVertices(i, adj);
            for (size_t j = 0; j < adj.size(); ++j) AddEdge(i, adj[j]);
        }
    }

    SetGraph(const SetGraph&) = delete;
    SetGraph& operator=(const SetGraph&) = delete;

    void AddEdge(size_t from, size_t to) override {
        edges_set.insert({from, to});
    }

    size_t VerticesCount() const noexcept override {
        return size;
    }

    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (auto& edge : edges_set) {
            if (edge.first == vertex) {
                vertices.push_back(edge.second);
            }
        }
    }

    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (auto& edge : edges_set) {
            if (edge.second == vertex) {
                vertices.push_back(edge.first);
            }
        }
    }
private:
    size_t size;
    std::set<std::pair<size_t, size_t>> edges_set;
};
