//
// Created by antarctica on 14.08.2021.
//

#ifndef LOG_SAFE_QUEUE_HPP
#define LOG_SAFE_QUEUE_HPP

#include <queue>
#include <mutex>

namespace log::utils {

    template <class T>
    class SafeQueue {
    public:
        SafeQueue() = default;
        ~SafeQueue() noexcept = default;

        [[nodiscard]] bool empty() const {
            return queue_.empty();
        }

        template <class... Args>
        void enqueue(Args... args) {
            std::lock_guard<std::mutex> lock(mut_);

            queue_.emplace(std::forward<Args>(args)...);
        }

        T dequeue() {
            std::lock_guard<std::mutex> lock(mut_);

            auto value = std::move(queue_.front());
            queue_.pop();
            return value;
        }

    private:
        std::queue<T> queue_;

        std::mutex mut_;
    };

} // namespace log::utils

#endif // LOG_SAFE_QUEUE_HPP
