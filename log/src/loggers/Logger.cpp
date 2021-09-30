//
// Created by antarctica on 30.07.2021.
//

#include "loggers/Logger.hpp"
#include "loggers/StdoutLogger.hpp"

namespace log {
    Logger::Logger() : global_logger_(std::make_shared<StdoutLogger>(Level::INFO)) {
    }

    Logger& Logger::get_instance() {
        static Logger instance;
        return instance;
    }

    BaseLoggerPtr Logger::get_global_logger() const noexcept {
        return global_logger_;
    }

    void Logger::set_global_logger(BaseLoggerPtr logger) {
        if(logger) {
            global_logger_ = std::move(logger);
        } else {
            throw std::invalid_argument("Null pointer passed");
        }
    }

    void init(BaseLoggerPtr logger) {
        Logger::get_instance().set_global_logger(std::move(logger));
    }

    void debug(const std::string& msg) {
        Logger::get_instance().get_global_logger()->debug(msg);
    }

    void info(const std::string& msg) {
        Logger::get_instance().get_global_logger()->info(msg);
    }

    void warning(const std::string& msg) {
        Logger::get_instance().get_global_logger()->warning(msg);
    }

    void error(const std::string& msg) {
        Logger::get_instance().get_global_logger()->error(msg);
    }

    void fatal(const std::string& msg) {
        Logger::get_instance().get_global_logger()->fatal(msg);
    }

    void flush() {
        Logger::get_instance().get_global_logger()->flush();
    }

} // namespace log
