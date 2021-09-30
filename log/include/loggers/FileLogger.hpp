//
// Created by antarctica on 06.03.2021.
//

#ifndef LOG_FILE_LOGGER_HPP
#define LOG_FILE_LOGGER_HPP

#include <fstream>
#include <memory>

#include "BaseLogger.hpp"

namespace log {

    class FileLogger final : public BaseLogger {
    public:
        FileLogger(const std::string& file_path, Level lvl, FormatterPtr formatter = nullptr);

        void flush() override;

    private:
        void log_impl(const std::string& msg) override;

    private:
        std::ofstream file_;
    };

    std::unique_ptr<FileLogger> create_file_logger(const std::string& file_path,
                                                   Level lvl,
                                                   FormatterPtr formatter = nullptr);

} // namespace log

#endif // LOG_FILE_LOGGER_HPP
