#include <logs/logger.h>

namespace pm
{

    Logger::Logger(std::wstring_view& message)
    {
        message_ = message;
    }

    Logger::Logger(std::string_view& message)
    {
        message_ = std::wstring(message.begin(), message.end());
    }

std::wstring_view Logger::ToWString() const
{
    return message_;
}

bool Logger::WriteLog(std::wstring_view& file_name) const
{
    std::wstring fname(file_name);
    std::ofstream ofs(fname, std::ios_base::app);
    ofs.write((char *)& message_[0], wcslen(&message_[0]) * 2);
    ofs.close();
}

bool Logger::WriteLog(std::string_view& file_name) const
{
    std::string fname(file_name);
    std::ofstream ofs(fname, std::ios_base::app);
    ofs.write((char *)& message_[0], wcslen(&message_[0]) * 2);
    ofs.close();
}


}