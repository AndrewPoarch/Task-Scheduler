#pragma once

#include "Node.hpp"

#include <vector>

class Graph {
  public:
    void AddEdge(size_t from, size_t to) {
        if (to == from) {
            return;
        }
        if (to >= graph_.size() || from >= graph_.size()) {
            AddVertex(std::max(to, from));
        }

        graph_[from].push_back(to);
    }

    std::vector<size_t> TopSort() {
        std::vector<bool> visited(graph_.size(), false);
        std::vector<size_t> order;
        for (int i = 0; i < graph_.size(); ++i) {
            if (!visited[i]) {
                DFS(i, visited, order);
            }
        }

        return order;
    }

    std::vector<size_t> TopSortOneVertex(size_t u) {
        std::vector<bool> visited(graph_.size(), false);
        std::vector<size_t> order;
        DFS(u, visited, order);

        return order;
    }

    void AddVertex(size_t u) {
        if (u >= graph_.size()) {
            graph_.resize(u + 1);
        }
    }

    bool HasCycle() const {
        std::vector<int> visited(graph_.size(), 0);
        bool has_cycle = false;

        for (auto i = 0; i < graph_.size(); ++i) {
            if (visited[i] != 2) {
                FindCycleDFS(i, visited, has_cycle);
            }
        }

        return has_cycle;
    }

    bool HasCycleOneVertex(size_t u) {
        std::vector<int> visited(graph_.size(), 0);
        bool has_cycle = false;

        FindCycleDFS(u, visited, has_cycle);

        return has_cycle;
    }

    size_t Size() {
        return graph_.size();
    }

  private:
    void DFS(size_t u, std::vector<bool>& vis, std::vector<size_t>& ans) {
        vis[u] = true;
        for (const auto& v : graph_[u]) {
            if (!vis[v]) {
                DFS(v, vis, ans);
            }
        }
        ans.push_back(u);
    }

    void FindCycleDFS(size_t u, std::vector<int>& visited, bool& has_cycle) const {
        if (has_cycle) {
            return;
        }
        if (visited[u] == 1) {
            has_cycle = true;
            return;
        }
        visited[u] = 1;
        for (size_t v : graph_[u]) {
            if (visited[v] != 2) {
                FindCycleDFS(v, visited, has_cycle);
            }
        }
        visited[u] = 2;
    }

    std::vector<std::vector<size_t>> graph_;
};
