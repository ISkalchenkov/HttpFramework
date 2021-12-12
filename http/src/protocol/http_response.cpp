#include <sstream>

#include "protocol/http_response.hpp"

namespace http {

    std::string HttpResponse::to_string() const {
        std::ostringstream response_data;
        response_data << version << ' ' << status << "\r\n";

        for (auto& header: headers) {
            response_data << header.first << ": " << header.second << "\r\n";
        }
        response_data << "\r\n";

        response_data << body;

        return response_data.str();
    }

} // namespace http
