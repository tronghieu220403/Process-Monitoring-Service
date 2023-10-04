
#include "logger.h"

namespace pm
{

    Logger::Logger(const std::string& message, const std::string& folder_path)
        : message_(message), folder_path_(folder_path)
    {

    };

    Logger::Logger(const std::string& message): message_(message)
    {
    };

    void Logger::SetMessage(const std::string_view& message)
    {
        message_ = message;
    }

    void Logger::AddMessage(const std::string_view& message)
    {
        message_.append(message);
    }

    void Logger::SetFolderPath(const std::string_view& folder_path)
    {
        folder_path_ = folder_path;
    }

    std::string Logger::GetMessage() const
    {
        return message_;
    }

    std::string Logger::GetFolderPath() const
    {
        return folder_path_;
    }

    void Logger::WriteLog() const
    {
        std::ofstream ofs(folder_path_ + "pm_logs.log", std::ios_base::app);
        ofs.write(&message_[0], strlen(&message_[0]));
        ofs.close();
    }

}