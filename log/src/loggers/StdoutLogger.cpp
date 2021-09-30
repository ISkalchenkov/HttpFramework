//
// Created by antarctica on 06.03.2021.
//

#include <iostream>
#include <utility>

#include "loggers/StdoutLogger.hpp"

namespace log {

    StdoutLogger::StdoutLogger(Level lvl, FormatterPtr formatter) noexcept
        : BaseLogger(lvl, std::move(formatter)) {
    }

    void StdoutLogger::flush() {
        std::cout.flush();
    }

    void StdoutLogger::log_impl(const std::string &msg) {
        std::cout << msg << '\n';
    }

    std::unique_ptr<StdoutLogger> create_stdout_logger(Level lvl, FormatterPtr formatter) {
        return std::make_unique<StdoutLogger>(lvl, std::move(formatter));
    }
} // namespace log
