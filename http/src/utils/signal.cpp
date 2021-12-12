#include "utils/signal.hpp"

namespace http {

    std::unordered_map<int, std::vector<Signal::Handler>> Signal::handlers_;

    void Signal::register_handler(int signum, const Handler& handler) {
        auto signum_handlers = handlers_.find(signum);
        if (signum_handlers == handlers_.end()) {
            handlers_.emplace(signum, std::vector<Signal::Handler>());

            signum_handlers = handlers_.find(signum);
            signum_handlers->second.emplace_back(handler);
        } else {
            signum_handlers->second.emplace_back(handler);
        }

        signal(signum, handle);
    }

    void Signal::handle(int signum) {
        auto signum_handlers = handlers_.find(signum);
        for (auto& handler: signum_handlers->second) {
            handler();
        }
    }

} // namespace http
