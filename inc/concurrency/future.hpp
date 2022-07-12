#ifndef CONCURRENCY_FUTURE_HPP_
#define CONCURRENCY_FUTURE_HPP_

#include <memory>

namespace concurrency {

/**
 * A customizable future. Unlike std::future, it permits the user
 * to give a custom policy for retrieving the result. Thus, it's easier
 * to create a "lazy future", a future that blocks until other thread fill
 * its result, or any future with a custom retrieving policy.
 */
template<typename T>
class Future {
public:

    /**
     * Creates a future with the given retrieving policy
     * @param policy callable policy for retrieving the future result
     */ 
    template<typename RetrievingPolicy>
    explicit Future(RetrievingPolicy policy)
        : impl_(std::make_unique<Derived<RetrievingPolicy>>(std::move(policy))), consumed_(false) {}

    /**
     * Waits if necessary for the computation to complete, and then retrives the result
     *
     * @return the computed result
     * @throws any exception thrown by the computation
     */
    T get() {
        if (consumed_)
            throw "future already consumed";

        consumed_ = true;

        return (*impl_)();
    }

private:
    struct Base {
        virtual ~Base() = default;
        virtual T operator()() = 0;
    };

    template<typename Impl>
    struct Derived : Base {

        Derived(Impl impl) : impl_(std::move(impl)) {}

        T operator()() override {
            return impl_();
        }

        Impl impl_;
    };

    std::unique_ptr<Base> impl_;
    bool consumed_;
};

} // namespace concurrenc

#endif // CONCURRENCY_FUTURE_HPP_