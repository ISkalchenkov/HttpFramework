//
// Created by antarctica on 02.08.2021.
//

#ifndef LOG_BASE_FORMATTER_HPP
#define LOG_BASE_FORMATTER_HPP

#include <string>

#include "Level.hpp"

namespace log {

    class BaseFormatter {
    public:
        using Modifications = unsigned int;

        explicit BaseFormatter(Modifications mods) noexcept;
        virtual ~BaseFormatter() noexcept = default;

        void set_mods(Modifications mods) noexcept;
        [[nodiscard]] bool is_mod(Modifications mod) const noexcept;

        virtual std::string format(const std::string& msg, Level lvl) = 0;

    private:
        Modifications mods_;
    };

} // namespace log

#endif // LOG_BASE_FORMATTER_HPP
