//
// Created by antarctica on 06.03.2021.
//

#include <iostream>

#include "loggers/StderrLogger.hpp"

namespace log {

    StderrLogger::StderrLogger(Level lvl, FormatterPtr formatter) noexcept
        : BaseLogger(lvl, std::move(formatter)) {
    }

    void StderrLogger::flush() {
        std::cerr.flush();
    }

    void StderrLogger::log_impl(const std::string &msg) {
        std::cerr << msg << '\n';
    }

    std::unique_ptr<StderrLogger> create_stderr_logger(Level lvl, FormatterPtr formatter) {
        return std::make_unique<StderrLogger>(lvl, std::move(formatter));
    }

} // namespace log
