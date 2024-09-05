#pragma once

#include "Any.hpp"

class TTask {
  public:
    virtual ~TTask() = default;

    virtual const Any& GetResult() = 0;

    virtual void Invoke() = 0;

  private:
};

template<typename Functor>
class TTaskZeroArg : public TTask {
  public:
    TTaskZeroArg() = delete;

    ~TTaskZeroArg() override = default;

    explicit TTaskZeroArg(const Functor& functor) :
        functor_(functor) {}

    const Any& GetResult() override {
        return object_;
    }

    void Invoke() override {
        object_.Put(functor_());
    }

  private:
    Functor functor_;
    Any object_;
};

template<typename Functor, typename First>
class TTaskOneArg : public TTask {
  public:
    TTaskOneArg() = delete;

    ~TTaskOneArg() override = default;

    explicit TTaskOneArg(const Functor& functor, const First& first) :
        functor_(functor),
        first_(first) {}

    const Any& GetResult() override {
        return object_;
    }

    void Invoke() override {
        object_.Put(functor_(first_));
    }

  private:
    Functor functor_;
    First first_;
    Any object_;
};

template<typename Functor, typename First, typename Second>
class TTaskTwoArgs : public TTask {
  public:
    TTaskTwoArgs() = delete;

    ~TTaskTwoArgs() override = default;

    explicit TTaskTwoArgs(const Functor& functor, const First& first, const Second& second) :
        functor_(functor),
        first_(first),
        second_(second) {}

    const Any& GetResult() override {
        return object_;
    }

    void Invoke() override {
        object_.Put(functor_(first_, second_));
    }

  private:
    Functor functor_;
    First first_;
    Second second_;
    Any object_;
};
