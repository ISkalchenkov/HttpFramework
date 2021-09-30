//
// Created by antarctica on 04.08.2021.
//

#ifndef LOG_PROPERTIES_HPP
#define LOG_PROPERTIES_HPP

namespace log::escapeCode {

    const char* RESET = "\x1b[0m";

    const char* BOLD = "\x1b[1m";
    const char* DEFAULT_WEIGHT = "\x1b[22m";

    const char* BLACK = "\x1b[30m";
    const char* RED = "\x1b[31m";
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[38;2;255;255;150m";
    const char* BLUE = "\x1b[34m";
    const char* MAGENTA = "\x1b[35m";
    const char* CYAN = "\x1b[36m";
    const char* WHITE = "\x1b[37m";
    const char* ORANGE = "\x1b[38;2;255;100;0m";
    const char* DEFAULT_COLOR = "\x1b[39m";

}

#endif // LOG_PROPERTIES_HPP
