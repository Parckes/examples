#include "igraph.hpp"

struct MatrixGraph: public IGraph {
public:
    MatrixGraph(size_t size): size(size), matrix(size, std::vector<size_t>(size, 0)) {};
    MatrixGraph(const IGraph* graph): size(graph->VerticesCount()),  matrix(size, std::vector<size_t>(size, 0)) {
        for (size_t i = 0; i < size; ++i) {
            std::vector<size_t> adj;
            graph->GetNextVertices(i, adj);
            for (size_t j = 0; j < adj.size(); ++j) AddEdge(i, adj[j]);
        }
    }

    MatrixGraph(const MatrixGraph&) = delete;
    MatrixGraph& operator=(const MatrixGraph&) = delete;

    void AddEdge(size_t from, size_t to) override {
        matrix[from][to] = 1;
    }

    size_t VerticesCount() const noexcept override {
        return size;
    }

    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (size_t i = 0; i < size; ++i) {
            if (matrix[vertex][i]) vertices.push_back(i);
        }
    }

    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept override {
        vertices = std::vector<size_t>();
        for (size_t i = 0; i < size; ++i) {
            if (matrix[i][vertex]) vertices.push_back(i);
        }
    }
private:
    size_t size;
    std::vector<std::vector<size_t>> matrix;
};
