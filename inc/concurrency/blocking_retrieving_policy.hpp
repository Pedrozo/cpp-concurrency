#ifndef CONCURRENCY_BLOCKING_RETRIEVING_POLICY_HPP_
#define CONCURRENCY_BLOCKING_RETRIEVING_POLICY_HPP_

#include <memory>
#include "blocking_container.hpp"
#include "value_or_exception.hpp"

namespace concurrency {

/**
 * A retrieving policy (usually used by Futures) that blocks until the internal container
 * gets filled by other thread.
 */
template<typename T>
class BlockingRetrievingPolicy {
public:

    /**
     * Creates a blocking retrieving policy with the given blocking container. The blocking
     * container will be used as the blocking mechanism of waiting the data.
     * 
     * @param cont the blocking container
     */
    explicit BlockingRetrievingPolicy(std::shared_ptr<BlockingContainer<ValueOrException<T>>> cont)
        : cont_(std::move(cont)) {}

    /**
     * Waits if necessary for the result, then returns it.
     * 
     * @return the result from the inner blocking container.
     */
    T operator()() {
        return cont_->consume().getOrThrow();
    }

private:
    std::shared_ptr<BlockingContainer<ValueOrException<T>>> cont_;
};

} // namespace concurrency

#endif // CONCURRENCY_BLOCKING_RETRIEVING_POLICY_HPP_