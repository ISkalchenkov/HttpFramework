//
// Created by antarctica on 03.08.2021.
//

#include <sstream>
#include <ctime>

#include "format/LogFormatter.hpp"
#include "Properties.hpp"

namespace log {

    LogFormatter::LogFormatter(Modifications mods) noexcept : BaseFormatter(mods) {
    }

    std::string LogFormatter::format(const std::string &msg, Level lvl) {
        std::ostringstream fmt_message;
        if (is_mod(mod::TIME)) {
            time_fmt(fmt_message);
        }

        if (is_mod(mod::COLOR)) {
            color_fmt(fmt_message, lvl);
        }

        if (is_mod(mod::WEIGHT)) {
            weight_fmt(fmt_message);
        }

        if (is_mod(mod::LEVEL)) {
            level_fmt(fmt_message, lvl);
        }

        fmt_message << msg;

        if (is_mod(mod::COLOR)) {
            fmt_message << escapeCode::DEFAULT_COLOR;
        }

        if (is_mod(mod::WEIGHT)) {
            fmt_message << escapeCode::DEFAULT_WEIGHT;
        }

        return fmt_message.str();
    }

    void LogFormatter::color_fmt(std::ostringstream& fmt_message, Level lvl) {
        switch (lvl) {
            case Level::DEBUG:
                fmt_message << escapeCode::CYAN;
                break;
            case Level::INFO:
                fmt_message << escapeCode::WHITE;
                break;
            case Level::WARNING:
                fmt_message << escapeCode::YELLOW;
                break;
            case Level::ERROR:
                fmt_message << escapeCode::ORANGE;
                break;
            case Level::FATAL:
                fmt_message << escapeCode::RED;
                break;
        }
    }

    void LogFormatter::weight_fmt(std::ostringstream& fmt_message) {
        fmt_message << escapeCode::BOLD;
    }

    void LogFormatter::time_fmt(std::ostringstream& fmt_message) {
        time_t now = std::time(nullptr);
        std::string current_time = std::ctime(&now);
        current_time.pop_back(); // delete \n
        fmt_message << '[' << current_time << "] : ";
    }

    void LogFormatter::level_fmt(std::ostringstream& fmt_message, Level lvl) {
        switch (lvl) {
            case Level::DEBUG:
                fmt_message << "DEBUG : ";
                break;

            case Level::INFO:
                fmt_message << "INFO  : ";
                break;

            case Level::WARNING:
                fmt_message << "WARN  : ";
                break;

            case Level::ERROR:
                fmt_message << "ERROR : ";
                break;

            case Level::FATAL:
                fmt_message << "FATAL : ";
                break;
        }
    }


} // namespace log
