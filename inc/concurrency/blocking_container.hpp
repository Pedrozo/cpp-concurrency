#ifndef CONCURRENCY_BLOCKING_CONTAINER_HPP_
#define CONCURRENCY_BLOCKING_CONTAINER_HPP_

#include <mutex>
#include <condition_variable>
#include <optional>

namespace concurrency {

/**
 * A thread-safe container that holds a single value.
 */
template<typename T>
class BlockingContainer {
public:

    /**
     * Creates an empty container
     */
    BlockingContainer() : data_(std::nullopt), mtx_(), cond_() {}

    /**
     * Checks if the container is holding a value.
     * 
     * @return true if the container has a value on it
     */
    operator bool() const {
        return bool(data_);
    }

    /**
     * Waits if necessary until the container receives data, then returns that data.
     * After that, the container will be empty.
     * 
     * @return the data stored in the container
     * @throws if the waiting gets cancelled
     */
    T consume() {
        std::unique_lock<std::mutex> lock(mtx_);

        cond_.wait(lock, [&] {
            return bool(data_);
        });

        if (!data_)
            throw "Cancelled"; // TODO: better exception

        T val = std::move(*data_);
        data_ = std::nullopt;
        return val;
    }

    /**
     * Inserts the give data into the container. If the contaner already has data,
     * it will be replaced.
     * 
     * @param value the supplied data to the container
     */
    void supply(T value) {
        std::unique_lock<std::mutex> lock(mtx_);

        data_ = std::optional<T>(std::move(value));

        lock.unlock();
        cond_.notify_one();
    }

    /**
     * Clears the container stored data, if exists.
     */
    void clear() {
        data_ = std::nullopt;
    }

private:
    std::optional<T> data_;
    std::mutex mtx_;
    std::condition_variable cond_;
};

} // namespace concurrency

#endif // CONCURRENCY_BLOCKING_CONTAINER_HPP_