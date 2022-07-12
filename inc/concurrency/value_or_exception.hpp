#ifndef CONCURRENCY_VALUE_OR_EXCEPTION_HPP_
#define CONCURRENCY_VALUE_OR_EXCEPTION_HPP_

#include <optional>

namespace concurrency {

/**
 * A container of a value or an exception. It will always have one, and only one, of these two.
 */
template<typename T>
class ValueOrException {
public:

    /**
     * Creates a container of the given value
     */
    ValueOrException(T value) : value_(std::move(value)), exception_() {}

    /**
     * Creates a container of the given exception
     */
    ValueOrException(std::exception_ptr exception) : value_(), exception_(std::move(exception)) {}

    /**
     * Gets its value, or throws its exception.
     * 
     * @return the value, if it's a container of a value
     * @throws the exception, if it's a container of an exception
     */
    T& getOrThrow() {
        if (exception_)
            std::rethrow_exception(exception_);
        
        return *value_;
    }

private:
    std::optional<T> value_;
    std::exception_ptr exception_;
};

} // namespace concurrency

#endif // CONCURRENCY_VALUE_OR_EXCEPTION_HPP_
