//
// Created by antarctica on 06.03.2021.
//

#ifndef LOG_BASE_LOGGER_HPP
#define LOG_BASE_LOGGER_HPP

#include <string>

#include "../Level.hpp"
#include "format/LogFormatter.hpp"

namespace log {

    using FormatterPtr = std::shared_ptr<LogFormatter>;

    class BaseLogger {
    public:
        explicit BaseLogger(Level lvl, FormatterPtr formatter = nullptr) noexcept;
        virtual ~BaseLogger() noexcept = default;

        void debug(const std::string& msg);
        void info(const std::string& msg);
        void warning(const std::string& msg);
        void error(const std::string& msg);
        void fatal(const std::string& msg);

        void set_level(Level lvl) noexcept;
        [[nodiscard]] Level level() const noexcept;
        virtual void flush() = 0;

    private:
        void log(const std::string &msg, Level lvl);
        virtual void log_impl(const std::string &msg) = 0;

        Level level_;
        FormatterPtr formatter_;
    };

} // namespace log

#endif // LOG_BASE_LOGGER_HPP
