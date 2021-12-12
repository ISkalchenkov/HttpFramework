#ifndef HTTP_SIGNAL_HPP
#define HTTP_SIGNAL_HPP

#include <csignal>
#include <functional>

namespace http {

    class Signal {
        using Handler = std::function<void()>;

    public:
        static void register_handler(int signum, const Handler& handler);

        template<class F, class... Args, class = std::enable_if_t<!std::is_invocable_v<F>>>
        static void register_handler(int signum, F&& f, Args&&... args) {
            register_handler(signum, std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        }

    private:
        static void handle(int signum);

    private:
        static std::unordered_map<int, std::vector<Handler>> handlers_;
};

} // namespace http

#endif // HTTP_SIGNAL_HPP
