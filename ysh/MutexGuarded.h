/*
 * MutexGuarded.h Created by Richard Buckley (C) 09/04/23
 */

/**
 * @file MutexGuarded.h
 * @author Richard Buckley <richard.buckley@ieee.org>
 * @version 1.0
 * @date 09/04/23
 * @brief 
 * @details
 */

#ifndef VE3YSH_UTIL_MUTEXGUARDED_H
#define VE3YSH_UTIL_MUTEXGUARDED_H

#include <mutex>
#include <functional>
#include <optional>

namespace ysh {

    /**
     * @class MutexGuarded
     * @brief A mutex guarded type.
     * @details A template which puts access to the guarded object behind a std::mutex lock. Access to the guarded
     * object is only allow by specifying a callback to the with() method of MutexGuarded. Before invoking the
     * callback with() creates a std::lock_guard on the std::mutex
     * @tparam Thing The underlying guarded type.
     */
    template<class Thing>
    class MutexGuarded {
    private:
        Thing mThing{};
        std::mutex mutable mMutex{};

    public:
        MutexGuarded() : mThing() {}
        ~MutexGuarded() = default;

        MutexGuarded(const MutexGuarded&) = delete;
        MutexGuarded& operator=(const MutexGuarded&) = delete;

        MutexGuarded(MutexGuarded&&)  noexcept = default;
        MutexGuarded& operator=(MutexGuarded&&)  noexcept = default;

        template<class ... Args>
        explicit MutexGuarded(Args...args) : mThing(args...) {}

        /**
         * @brief Get lock and invoke callback with thing.
         * @tparam Callback type of callback
         * @param callback The callback
         * @return the value returned by the callback.
         */
        template<class Callback>
        decltype(auto) with(Callback callback) {
            const std::lock_guard<std::mutex> lockGuard{mMutex};
            return callback(mThing);
        }

        /**
         * @brief Get lock and invoke callback with thing, const version.
         * @tparam Callback type of callback
         * @param callback The callback
         * @return the value returned by the callback.
         */
        template<class Callback>
        decltype(auto) with(Callback callback) const {
            const std::lock_guard<std::mutex> lockGuard{mMutex};
            return callback(mThing);
        }

        /**
         * @brief Try to get lock. If lock is acquired invoke callback with thing.
         * @tparam Callback type of callback
         * @param callback the callback
         * @return std::optional with the callback return if the lock was acquired, empty otherwise.
         */
        template<class Callback>
        auto try_with(Callback callback) {
            using result_t = typename std::result_of<Callback(Thing&)>::type;
            std::optional<result_t> result{};
            std::unique_lock<std::mutex> lockGuard(mMutex,std::defer_lock);
            if (lockGuard.try_lock())
                return std::make_optional<>(callback(mThing));
            return result;
        }

        /**
         * @brief Try to get lock. If lock is acquired invoke callback with thing, const version
         * @tparam Callback type of callback
         * @param callback the callback
         * @return std::optional with the callback return if the lock was acquired, empty otherwise.
         */
        template<class Callback>
        auto try_with(Callback callback) const {
            using result_t = typename std::result_of<Callback(Thing&)>::type;
            std::optional<result_t> result{};
            std::unique_lock<std::mutex> lockGuard(mMutex,std::defer_lock);
            if (lockGuard.try_lock())
                return std::make_optional<>(callback(mThing));
            return result;
        }
    };

} // ysh

#endif //VE3YSH_UTIL_MUTEXGUARDED_H
