#include "HttpServer.hpp"
#include "protocol/http.hpp"
#include "filesystem"
#include "loggers/ThreadedLogger.hpp"

HttpServer::HttpServer(std::string_view address, uint16_t port, int max_conn, std::string_view document_root)
    : http::Server(address, port, max_conn)
    , document_root_(document_root)
    , response_status(http::response_status::S_200_OK) {
}

http::HttpResponse HttpServer::on_request(const http::HttpRequest& request) {
    // Can use log library there
    // Ex: log::info_tl("Example");
    response_status = http::response_status::S_200_OK;

    validate_method(request.method);
    validate_path(request.path);
    validate_version(request.version);

    http::HttpResponse response;
    response.version = request.version;

    std::string file_path = document_root_ + request.path;
    if (is_file_available(file_path)) {
        response.files.emplace(file_path);
    }

    response.status = response_status;
    return response;
}

bool HttpServer::validate_method(std::string_view method) {
    if (method != http::method::GET) {
        response_status = http::response_status::S_405_MNA;
        return false;
    }
    return true;
}

bool HttpServer::validate_path(std::string_view path) {
    if (path.find("../") != std::string_view::npos) {
        response_status = http::response_status::S_403_F;
        return false;
    }
    return true;
}

bool HttpServer::validate_version(std::string_view version) {
    if (!version.starts_with("HTTP/")) {
        response_status = http::response_status::S_400_BR;
        return false;
    }
    return true;
}

bool HttpServer::is_file_available(std::string_view file_path) {
    if (std::filesystem::is_directory(file_path)) {
        response_status = http::response_status::S_403_F;
    }
    if (!std::filesystem::exists(file_path)) {
        response_status = http::response_status::S_404_NF;
    }
    return response_status == http::response_status::S_200_OK;
}
