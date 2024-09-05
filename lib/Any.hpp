#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <memory>

class BaseMemoryManager {
  public:
    virtual ~BaseMemoryManager() = default;

    virtual void Delete(void* ptr) const = 0;

    virtual void* Copy(void* ptr) const = 0;

    [[nodiscard]] virtual BaseMemoryManager* Create() const = 0;

    [[nodiscard]] virtual const char* GetTypeName() const = 0;
};

template<typename T>
class MemoryManager;

template<typename T>
class MemoryManager : public BaseMemoryManager {
  public:
    void Delete(void* ptr) const override {
        auto t_ptr = static_cast<T*>(ptr);
        delete t_ptr;
    }

    void* Copy(void* ptr) const override {
        auto temp_ptr = static_cast<T*>(ptr);

        return new T(*temp_ptr);
    }

    [[nodiscard]] BaseMemoryManager* Create() const override {
        return new MemoryManager<T>();
    }

    [[nodiscard]] const char* GetTypeName() const override {
        return typeid(T).name();
    }
};

template<>
class MemoryManager<void> : public BaseMemoryManager {
  public:
    void Delete(void* ptr) const override {}

    void* Copy(void* ptr) const override {
        return nullptr;
    }

    [[nodiscard]] BaseMemoryManager* Create() const override {
        return new MemoryManager<void>();
    }

    [[nodiscard]] const char* GetTypeName() const override {
        return typeid(void).name();
    }
};

class Any {
  public:
    Any() :
        m_ptr_(nullptr),
        manager_ptr_(nullptr) {
        manager_ptr_ = new MemoryManager<void>();
    }
    Any(const Any& other) :
        m_ptr_(nullptr),
        manager_ptr_(nullptr) {
        manager_ptr_ = other.manager_ptr_->Create();
        m_ptr_ = manager_ptr_->Copy(other.m_ptr_);
    }

    ~Any() {
        Clear();
        delete manager_ptr_;
    }

    Any& operator=(const Any& other) {
        if (this != &other) {
            Clear();
            delete manager_ptr_;
            manager_ptr_ = other.manager_ptr_->Create();
            m_ptr_ = manager_ptr_->Copy(other.m_ptr_);
        }

        return *this;
    }

    void Clear() {
        manager_ptr_->Delete(m_ptr_);
        delete manager_ptr_;

        m_ptr_ = nullptr;
        manager_ptr_ = new MemoryManager<void>();
    }

    template<typename T>
    T& Get() {
        if (strcmp(manager_ptr_->GetTypeName(), typeid(T).name()) != 0) {
            throw std::bad_cast();
        }

        return *static_cast<T*>(m_ptr_);
    }

    template<typename T>
    const T& Get() const {
        if (strcmp(manager_ptr_->GetTypeName(), typeid(T).name()) != 0) {
            throw std::bad_cast();
        }

        return *static_cast<T*>(m_ptr_);
    }

    template<typename T>
    operator T&() {
        return Get<T>();
    }

    template<typename T>
    operator const T&() const {
        return Get<T>();
    }

    template<typename T>
    void Put(const T& t) {
        Clear();
        delete manager_ptr_;

        m_ptr_ = new T(t);
        manager_ptr_ = new MemoryManager<T>();
    }

  private:
    void* m_ptr_;
    BaseMemoryManager* manager_ptr_;
};
