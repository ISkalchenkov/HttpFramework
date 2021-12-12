#ifndef HTTP_HTTP_HPP
#define HTTP_HTTP_HPP

#include <string_view>

namespace http {

    namespace method {

        constexpr std::string_view GET = "GET";
        constexpr std::string_view HEAD = "HEAD";
        constexpr std::string_view POST = "POST";

    } // namespace method

    namespace version {
        constexpr std::string_view V0_9 = "HTTP/0.9";
        constexpr std::string_view V1_0 = "HTTP/1.0";
        constexpr std::string_view V1_1 = "HTTP/1.1";
    } // namespace version

    namespace response_status {

        constexpr std::string_view S_200_OK = "200 OK";

        constexpr std::string_view S_400_BR = "400 Bad Request";
        constexpr std::string_view S_403_F = "403 Forbidden";
        constexpr std::string_view S_404_NF = "404 Not Found";
        constexpr std::string_view S_405_MNA = "405 Method Not Allowed";

        constexpr std::string_view S_505_VNS = "HTTP Version Not Supported";
    } // namespace response_status


} // namespace http

#endif // HTTP_HTTP_HPP
