//
// Created by antarctica on 03.08.2021.
//

#ifndef LOG_LOG_FORMATTER_HPP
#define LOG_LOG_FORMATTER_HPP

#include "BaseFormatter.hpp"

namespace log {

    namespace mod {
        enum Mod {
            NONE = 0,

            LEVEL = 1,
            COLOR = 1 << 1,
            WEIGHT = 1 << 2,
            TIME = 1 << 3,

            ALL = ~0
        };
    } // namespace mod

    class LogFormatter : public BaseFormatter {
    public:
        explicit LogFormatter(Modifications mods) noexcept;

        std::string format(const std::string& msg, Level lvl) override;

    private:
        void color_fmt(std::ostringstream& fmt_message, Level lvl);
        void weight_fmt(std::ostringstream& fmt_message);
        void time_fmt(std::ostringstream& fmt_message);
        void level_fmt(std::ostringstream& fmt_message, Level lvl);
    };

}

#endif // LOG_LOG_FORMATTER_HPP
