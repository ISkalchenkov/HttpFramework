//
// Created by antarctica on 06.03.2021.
//

#ifndef LOG_STDERR_LOGGER_HPP
#define LOG_STDERR_LOGGER_HPP

#include <memory>

#include "../Level.hpp"
#include "BaseLogger.hpp"

namespace log {

    class StderrLogger final : public BaseLogger {
    public:
        explicit StderrLogger(Level lvl, FormatterPtr formatter = nullptr) noexcept;

        void flush() override;

    private:
        void log_impl(const std::string& msg) override;
    };

    std::unique_ptr<StderrLogger> create_stderr_logger(Level lvl, FormatterPtr formatter = nullptr);

} // namespace log

#endif // LOG_STDERR_LOGGER_HPP
