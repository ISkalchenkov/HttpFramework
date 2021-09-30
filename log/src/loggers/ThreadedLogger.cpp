//
// Created by antarctica on 11.08.2021.
//

#include "loggers/ThreadedLogger.hpp"

#include <utility>

namespace log {

    ThreadedLogger::ThreadedLogger()
        : worker_([this]() {this->work();})
        , done_(false) {
    }

    ThreadedLogger::~ThreadedLogger() noexcept {
        stop();
    }

    ThreadedLogger& ThreadedLogger::get_instance() {
        static ThreadedLogger instance;
        return instance;
    }

    void ThreadedLogger::start() {
        std::lock_guard<std::mutex> lock(mut_);
        stop_worker();
        done_ = false;
        worker_ = std::thread([this]() {this->work();});
    }

    void ThreadedLogger::stop() {
        std::lock_guard<std::mutex> lock(mut_);
        stop_worker();
    }

    void ThreadedLogger::enqueue_log(const std::string& msg, CallbackType cb) {
        queue_.enqueue(msg, std::move(cb));
        cv_.notify_one();
    }

    void ThreadedLogger::log() {
        auto data = queue_.dequeue();
        data.second(data.first);
    }

    void ThreadedLogger::work() {
        while (!done_) {
            {
                std::unique_lock<std::mutex> lock(lock_worker_);
                cv_.wait(lock, [this]() {
                    return (!queue_.empty() || done_);
                });
            }

            if (!queue_.empty()) {
                log();
            }
        }

        while (!queue_.empty()) {
            log();
        }
    }

    void ThreadedLogger::stop_worker() {
        done_ = true;
        if (worker_.joinable()) {
            cv_.notify_one();
            worker_.join();
        }
    }

    void debug_tl(const std::string& msg) {
        ThreadedLogger::get_instance().enqueue_log(msg, debug);
    }

    void info_tl(const std::string& msg) {
        ThreadedLogger::get_instance().enqueue_log(msg, info);
    }

    void warning_tl(const std::string& msg) {
        ThreadedLogger::get_instance().enqueue_log(msg, warning);
    }

    void error_tl(const std::string& msg) {
        ThreadedLogger::get_instance().enqueue_log(msg, error);
    }

    void fatal_tl(const std::string& msg) {
        ThreadedLogger::get_instance().enqueue_log(msg, fatal);
    }

} // namespace log
