#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <unordered_map>
#include <queue>

namespace http {

    class HttpResponse {
    public:
        std::string version;
        std::string_view status;
        std::unordered_map<std::string_view, std::string_view> headers;
        std::string body;
        std::queue<std::string> files;

        std::string to_string() const;
    };

} // namespace http

#endif // HTTP_RESPONSE_HPP
