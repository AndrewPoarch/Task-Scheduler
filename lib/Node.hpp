#pragma once

#include "Task.hpp"
#include <vector>

struct NodeIndex {
    size_t id_;
};

class Node {
  public:
    Node() = delete;

    Node(const Node& other) = delete;

    ~Node() = default;

    Node(Node&& other) noexcept = default;

    Node& operator=(Node&& other) noexcept = default;

    template<typename Functor>
    explicit Node(const Functor& functor) :
        task_ptr_(std::make_unique<TTaskZeroArg<Functor>>(functor)) {}

    template<typename Functor, typename First>
    Node(const Functor& functor, const First& first) :
        task_ptr_(std::make_unique<TTaskOneArg<Functor, First>>(functor, first)) {}

    template<typename Functor, typename First, typename Second>
    Node(const Functor& functor, const First& first, const Second& second) :
        task_ptr_(std::make_unique<TTaskTwoArgs<Functor, First, Second>>(functor, first, second)) {}

    [[nodiscard]] bool WasExecuted() const {
        return was_executed_;
    }

    template<typename T>
    const T& GetResult() const {
        if (!was_executed_) {
            throw std::logic_error("Task wasn't executed.... Correct your logic (:");
        }
        return task_ptr_->GetResult().Get<T>();
    }

    void Execute() {
        if (!was_executed_) {
            task_ptr_->Invoke();
            was_executed_ = true;
        }
    }

  private:
    bool was_executed_ = false;
    std::unique_ptr<TTask> task_ptr_;
};
