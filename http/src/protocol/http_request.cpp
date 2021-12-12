#include "protocol/http_request.hpp"

namespace http {

    HttpRequest::HttpRequest(std::string_view request_data) : request_data_(request_data) {
        parse_method();
        parse_path();
        parse_version();
        parse_headers();
        parse_body();
    }

    void HttpRequest::parse_method() {
        size_t pos = request_data_.find(" /");
        method = request_data_.substr(0, pos);
        request_data_.remove_prefix(pos + 1);
    }

    void HttpRequest::parse_path() {
        size_t pos = request_data_.find(" HTTP/");
        path = decode_uri(static_cast<std::string>(request_data_.substr(0, pos)));
        size_t query_strings_pos = path.find('?');
        if (query_strings_pos != std::string::npos) {
            parse_query_strings(path.substr(query_strings_pos + 1)); // +1 - delete '?'
            path = path.substr(0, query_strings_pos);
        }
        request_data_.remove_prefix(pos + 1);
    }

    void HttpRequest::parse_query_strings(std::string_view query_string) {
        while (true) {
            size_t param_name_pos = query_string.find('=');
            if (param_name_pos == std::string_view::npos)
                break;

            std::string_view param_name = query_string.substr(0, param_name_pos);
            query_string.remove_prefix(param_name_pos + 1);

            size_t param_value_pos = query_string.find('&');
            std::string_view param_value;
            if (param_value_pos == std::string_view::npos) {
                param_value = query_string;
            } else {
                param_value = query_string.substr(0, param_value_pos);
                query_string.remove_prefix(param_value_pos + 1);
            }
            query_strings.emplace(param_name, param_value);
        }
    }

    void HttpRequest::parse_version() {
        size_t lf_pos = request_data_.find('\n');
        if (request_data_.at(lf_pos - 1) == '\r') {
            version = request_data_.substr(0, lf_pos - 1);
        } else {
            version = request_data_.substr(0, lf_pos);
        }

        request_data_.remove_prefix(lf_pos + 1);
    }

    void HttpRequest::parse_headers() {
        size_t lf_pos;
        while ((lf_pos = request_data_.find('\n')) != std::string_view::npos &&
            !(request_data_.starts_with('\r') || (request_data_.starts_with('\n')))) {

            size_t header_name_pos = request_data_.find(':');
            std::string_view header_name = request_data_.substr(0, header_name_pos);

            size_t header_value_begin_pos = request_data_.find_first_not_of(' ', header_name_pos + 1); // +1 - skip ':'
            std::string_view header_value;
            if (request_data_.at(lf_pos - 1) == '\r') {
                header_value = request_data_.substr(header_value_begin_pos, (lf_pos - 1) - header_value_begin_pos);
            } else {
                header_value = request_data_.substr(header_value_begin_pos, lf_pos - header_value_begin_pos);
            }

            request_data_.remove_prefix(lf_pos + 1);
            headers.emplace(header_name, header_value);
        }

        if (lf_pos == std::string_view::npos)
            throw std::runtime_error("Error parsing headers");

        request_data_.remove_prefix(lf_pos + 1); // skip second CR/LN
    }

    /*    void HttpRequest::parse_headers() {
        while (request_data_.find('\n') != std::string_view::npos &&
               !(request_data_.starts_with('\r') || (request_data_.starts_with('\n')))) {

            size_t header_name_pos = request_data_.find(':');
            std::string_view header_name = request_data_.substr(0, header_name_pos);

            request_data_.remove_prefix(header_name_pos + 1); // remove header name and ':'
            request_data_.remove_prefix(request_data_.find_first_not_of(' ')); // remove spaces

            std::string_view header_value;
            size_t lf_pos = request_data_.find('\n');
            if (request_data_.at(lf_pos - 1) == '\r') {
                header_value = request_data_.substr(0, lf_pos - 1);
            } else {
                header_value = request_data_.substr(0, lf_pos);
            }

            request_data_.remove_prefix(lf_pos + 1);
            headers.emplace(header_name, header_value);
        }

        size_t lf_pos = request_data_.find('\n');
        if (lf_pos == std::string_view::npos)
            throw std::runtime_error("Error parsing headers");

        request_data_.remove_prefix(lf_pos + 1); // go to request body
    }*/

    void HttpRequest::parse_body() {
        body = request_data_;
    }

    std::string HttpRequest::decode_uri(const std::string& uri) {
        std::string result;
        char symb;
        unsigned code;

        for (int i = 0; i < uri.length(); i++) {
            if (uri[i] != '%') {
                if(uri[i] == '+')
                    result += ' ';
                else
                    result += uri[i];
            } else {
                sscanf(uri.substr(i + 1, 2).c_str(), "%x", &code);
                symb = static_cast<char>(code);
                result += symb;
                i = i + 2;
            }
        }
        return result;
    }

} // namespace http
