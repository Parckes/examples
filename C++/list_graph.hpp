#include "igraph.hpp"

struct ListGraph: public IGraph {
public:
    ListGraph(size_t size): size(size), adj_list(size, std::vector<size_t>()){};
    ListGraph(const IGraph* graph): size(graph->VerticesCount()), adj_list(size, std::vector<size_t>()) {
        for (size_t i = 0; i < size; ++i) {
            std::vector<size_t> adj;
            graph->GetNextVertices(i, adj);
            for (int j = 0; j < adj.size(); ++j) AddEdge(i, adj[j]);
        }
    }

    ListGraph(const ListGraph&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;

    void AddEdge(size_t from, size_t to) override {
        for (size_t i = 0; i < adj_list[from].size(); ++i) {
            if (adj_list[from][i] == to) return;
        }
        adj_list[from].push_back(to);
    }

    size_t VerticesCount() const noexcept override {
        return size;
    }

    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (size_t i = 0; i < adj_list[vertex].size(); ++i) {
            vertices.push_back(adj_list[vertex][i]);
        }
    }

    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (size_t i = 0; i < adj_list.size(); ++i) {
            for (size_t j = 0; j < adj_list[i].size(); ++j) {
                if (adj_list[i][j] == vertex) vertices.push_back(i);
            }
        }
    }
private:
    size_t size;
    std::vector<std::vector<size_t>> adj_list;
};
