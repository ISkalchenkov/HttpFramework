#include <iostream>

#include "HttpServer.hpp"
#include "utils/signal.hpp"

#include "loggers/ThreadedLogger.hpp"
#include "loggers/StdoutLogger.hpp"

int main() {

    // std::ofstream out;
    // std::string path = "/home/antarctica/server/file2.html";
    // out.open(path);
    // uintmax_t file_size = 0;
    // for (size_t i = 0; file_size < 52428800; ++i) {
    //     out << std::to_string(i) << std::endl;
    //     file_size = std::filesystem::file_size(path);
    // }
    //
    // exit(0);

    try {

        log::init(log::create_stdout_logger(log::Level::DEBUG,
                                            std::make_shared<log::LogFormatter>(log::mod::COLOR)));

        HttpServer server("127.0.0.1", 80, 128, "/home/antarctica/server");

        http::Signal::register_handler(SIGINT, &HttpServer::signal_handler, &server);
        http::Signal::register_handler(SIGTERM, &HttpServer::signal_handler, &server);

        server.run(http::TimeoutType(5000), http::TimeoutType(5000), 2);

    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}
