//
// Created by antarctica on 06.03.2021.
//

#ifndef LOG_STDOUT_LOGGER_HPP
#define LOG_STDOUT_LOGGER_HPP

#include <memory>

#include "../Level.hpp"
#include "BaseLogger.hpp"

namespace log {

    class StdoutLogger final : public BaseLogger {
    public:
        explicit StdoutLogger(Level lvl, FormatterPtr formatter = nullptr) noexcept;

        void flush() override;

    private:
        void log_impl(const std::string& msg) override;
    };

    std::unique_ptr<StdoutLogger> create_stdout_logger(Level lvl, FormatterPtr formatter = nullptr);

} // namespace log

#endif // LOG_STDOUT_LOGGER_HPP
