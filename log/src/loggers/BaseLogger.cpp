//
// Created by antarctica on 06.03.2021.
//

#include <memory>

#include "loggers/BaseLogger.hpp"

namespace log {

    BaseLogger::BaseLogger(Level lvl, FormatterPtr formatter) noexcept
        : level_(lvl)
        , formatter_(std::move(formatter)) {
    }

    void BaseLogger::debug(const std::string& msg) {
        log(msg, Level::DEBUG);
    }

    void BaseLogger::info(const std::string& msg) {
        log(msg, Level::INFO);
    }

    void BaseLogger::warning(const std::string& msg) {
        log(msg, Level::WARNING);
    }

    void BaseLogger::error(const std::string& msg) {
        log(msg, Level::ERROR);
    }

    void BaseLogger::fatal(const std::string& msg) {
        log(msg, Level::FATAL);
    }

    void BaseLogger::set_level(const Level lvl) noexcept {
        level_ = lvl;
    }

    Level BaseLogger::level() const noexcept {
        return level_;
    }

    void BaseLogger::log(const std::string &msg, Level lvl) {
        if (lvl < level_) {
            return;
        }

        if (formatter_) {
            if (!formatter_->is_mod(mod::NONE)) {
                log_impl(formatter_->format(msg, lvl));
                return;
            }
        }
        log_impl(msg); // if formatter_ is nullptr or none mods
    }

} // namespace log
