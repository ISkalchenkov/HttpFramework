//
// Created by antarctica on 11.08.2021.
//

#ifndef LOG_THREADED_LOGGER_HPP
#define LOG_THREADED_LOGGER_HPP

#include <thread>
#include <functional>
#include <condition_variable>

#include "loggers/Logger.hpp"
#include "utils/SafeQueue.hpp"

namespace log {

    class ThreadedLogger {
        using CallbackType = std::function<void(std::string&)>;
        using QueueDataType = std::pair<std::string, CallbackType>;

    public:
        ThreadedLogger(const ThreadedLogger& rhs) = delete;
        ThreadedLogger& operator=(const ThreadedLogger& rhs) = delete;

        static ThreadedLogger& get_instance();
        void start();
        void stop();
        void enqueue_log(const std::string& msg, CallbackType cb);

    private:
        ThreadedLogger();
        ~ThreadedLogger() noexcept;

        void work();
        void stop_worker();
        void log();

    private:
        utils::SafeQueue<QueueDataType> queue_;

        std::thread worker_;
        std::mutex mut_;
        std::mutex lock_worker_;
        std::condition_variable cv_;
        bool done_;
    };

    void debug_tl(const std::string& msg);
    void info_tl(const std::string& msg);
    void warning_tl(const std::string& msg);
    void error_tl(const std::string& msg);
    void fatal_tl(const std::string& msg);

} // namespace log

#endif // LOG_THREADED_LOGGER_HPP
