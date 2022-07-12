#ifndef CONCURRENCY_PROMISE_HPP_
#define CONCURRENCY_PROMISE_HPP_

#include "blocking_container.hpp"
#include "blocking_retrieving_policy.hpp"
#include "value_or_exception.hpp"

namespace concurrency {

template<typename T>
class Promise {
public:

    Promise()
        : container_(std::make_shared<BlockingContainer<ValueOrException<T>>>()),
          completed_(false) {}

    void setValue(T value) {
        if (completed_)
            throw "Promise already completed";

        completed_ = true;

        container_->supply(std::move(value));
    }

    void setException(std::exception_ptr exception) {
        if (completed_)
            throw "Promise already completed";

        completed_ = true;

        container_->supply(std::move(exception));
    }

    Future<T> getFuture() {
        // TODO: do not return more than 1 future
        return Future<T>(BlockingRetrievingPolicy<T>(container_));
    }

private:
    std::shared_ptr<BlockingContainer<ValueOrException<T>>> container_;
    bool completed_;
};

} // namespace concurrency

#endif // CONCURRENCY_PROMISE_HPP_