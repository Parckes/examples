#include "igraph.hpp"


struct ArcGraph: public IGraph {
public:
    ArcGraph(size_t size): size(size) {};
    ArcGraph(const IGraph* graph): size(graph->VerticesCount()) {
        for (size_t i = 0; i < size; ++i) {
            std::vector<size_t> adj;
            graph->GetNextVertices(i, adj);
            for (int j = 0; j < adj.size(); ++j) AddEdge(i, adj[j]);
        }
    }
    ArcGraph(const ArcGraph&) = delete;
    ArcGraph& operator=(const ArcGraph&) = delete;

    void AddEdge(size_t from, size_t to) override {
        for (auto& e : edges_list) {
            if (e == std::pair<size_t, size_t>({from, to})) return;
        }
        edges_list.push_back({from, to});
    }

    size_t VerticesCount() const noexcept override {
        return size;
    }

    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (auto& edge : edges_list) {
            if (edge.first == vertex) {
                vertices.push_back(edge.second);
            }
        }
    }

    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (auto& edge : edges_list) {
            if (edge.second == vertex) {
                vertices.push_back(edge.first);
            }
        }
    }

    ~ArcGraph() = default;
private:
    size_t size;
    std::vector<std::pair<size_t, size_t>> edges_list;
};
