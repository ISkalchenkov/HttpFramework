//
// Created by antarctica on 06.03.2021.
//

#include <loggers/FileLogger.hpp>

namespace log {

    FileLogger::FileLogger(const std::string& file_path, Level lvl, FormatterPtr formatter)
        : file_(file_path)
        , BaseLogger(lvl, std::move(formatter)) {
        if (!file_) {
            throw std::runtime_error("Can't open file '" + file_path + "'");
        }
    }

    void FileLogger::flush() {
        file_.flush();
    }

    void FileLogger::log_impl(const std::string& msg) {
        file_ << msg << '\n';
    }

    std::unique_ptr<FileLogger> create_file_logger(const std::string& file_path,
                                                   Level lvl,
                                                   FormatterPtr formatter) {
        return std::make_unique<FileLogger>(file_path, lvl, std::move(formatter));
    }

} // namespace log
