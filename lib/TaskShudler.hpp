#pragma once

#include <ranges>

#include "Graph.hpp"

template<typename T>
class FutureResult {
  public:
    FutureResult() = delete;

    FutureResult(const std::vector<Node>& nodes, size_t id) :
        id(id),
        nodes_(nodes) {}
    operator const T&() const {
        return nodes_[id].GetResult<T>();
    }
  private:
    size_t id;
    const std::vector<Node>& nodes_;
};

class TTaskScheduler {
  public:
    TTaskScheduler() = default;

    template<typename Functor, typename First, typename Second>
    NodeIndex add(const Functor& functor, const First& first, const Second& second) {
        nodes_.emplace_back(functor, first, second);
        graph_.AddVertex(nodes_.size() - 1);

        return {nodes_.size() - 1};
    }

    template<typename Functor, typename First>
    NodeIndex add(const Functor& functor, const First& first) {
        nodes_.emplace_back(functor, first);
        graph_.AddVertex(nodes_.size() - 1);

        return {nodes_.size() - 1};
    }

    template<typename Functor>
    NodeIndex add(const Functor& functor) {
        nodes_.emplace_back(functor);
        graph_.AddVertex(nodes_.size() - 1);

        return {nodes_.size() - 1};
    }

    template<typename T>
    FutureResult<T> getFutureResult(const NodeIndex& index) {
        if (index.id_ > nodes_.size() -1) {
            throw std::logic_error("index > size \n");
        }
        graph_.AddEdge(nodes_.size() - 1, index.id_);

        return FutureResult<T>(nodes_, index.id_);
    }

    template<typename T>
    const T& getResult(const NodeIndex& index) {
        if (graph_.HasCycleOneVertex(index.id_)) {
            throw std::logic_error("Found cycle in Task № " + std::to_string(index.id_) + ".....");
        }

        if (!nodes_[index.id_].WasExecuted()) {
            ExecuteFromCurrent(graph_.TopSortOneVertex(index.id_));
        }

        return nodes_[index.id_].GetResult<T>();
    }

    void executeAll() {
        if (graph_.HasCycle()) {
            throw std::logic_error("Found cycle..... :(");
        }
        ExecuteFromCurrent(graph_.TopSort());
    }

  private:
    void ExecuteFromCurrent(const std::vector<size_t>& indexes) {
        for (const auto& index : indexes) {
            nodes_[index].Execute();
        }
    }

    std::vector<Node> nodes_;
    Graph graph_;
};
