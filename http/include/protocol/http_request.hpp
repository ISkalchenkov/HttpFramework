#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string_view>
#include <unordered_map>
#include <string>

namespace http {

    class HttpRequest {
    public:
        explicit HttpRequest(std::string_view request_data);

    public:
        std::string_view method;
        std::string path;
        std::unordered_map<std::string, std::string> query_strings;
        std::string_view version;
        std::unordered_map<std::string_view, std::string_view> headers;
        std::string_view body;

    private:
        std::string_view request_data_;

    private:
        void parse_method();
        void parse_path();
        void parse_query_strings(std::string_view query_string);
        void parse_version();
        void parse_headers();
        void parse_body();

        static std::string decode_uri(const std::string& uri);
    };

} // namespace http

#endif // HTTP_REQUEST_HPP
