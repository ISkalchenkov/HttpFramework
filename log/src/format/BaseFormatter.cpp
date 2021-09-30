//
// Created by antarctica on 02.08.2021.
//

#include "format/BaseFormatter.hpp"

namespace log {

    BaseFormatter::BaseFormatter(Modifications mods) noexcept
        : mods_(mods) {
    }

    void BaseFormatter::set_mods(Modifications mods) noexcept {
        mods_ = mods;
    }

    bool BaseFormatter::is_mod(Modifications mod) const noexcept {
        return (mod != 0) ? (mod & mods_) : (mod == mods_); // 0 is mean none modifications
    }

} // namespace log
