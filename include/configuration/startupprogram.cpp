#include "startupprogram.h"

namespace pm
{
    StartUpProgram::StartUpProgram(const std::string& exe_full_path) : exe_full_path_(exe_full_path)
    {
    }

    void StartUpProgram::SetThisProgram()
    {
        #ifdef _WIN32
            std::wstring w_exe_full_path;
            w_exe_full_path.resize(MAX_PATH);
            GetModuleFileNameW(nullptr, &w_exe_full_path[0], MAX_PATH);
            exe_full_path_ = std::string(w_exe_full_path.begin(), w_exe_full_path.end());
        #elif __linux__
            exe_full_path_.resize(10000);
            int sz = readlink("/proc/self/exe", exe_full_path_.data(), exe_full_path_.size());
            exe_full_path_.resize(sz);
        #endif
    }

    void StartUpProgram::SetProgram(const std::string_view& exe_full_path)
    {
        exe_full_path_ = exe_full_path;
    }

    bool StartUpProgram::Register()
    {
        bool success = false;
        #ifdef _WIN32
            
            auto w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());

            long result = 0;
            
            HKEY h_key = nullptr;
            result = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, nullptr, 0, (KEY_WRITE | KEY_READ), nullptr, &h_key, nullptr);
            
            success = (result == 0);
            
            if (success)
            {
                result = RegSetValueExW(h_key, L"ProcessMonitoring",0, REG_SZ, (BYTE *)&w_exe_full_path[0], 
                    static_cast<DWORD>(wcslen(&w_exe_full_path[0]) + 1) * 2);
                success = (result == 0);
            }

            if (h_key != nullptr)
            {
                RegCloseKey(h_key);
                h_key = nullptr;
            }

            return success;
        #elif __linux__

            std::string user_autostart_path = GetCurrentUserPath() + "/.config/autostart";

            if (CreateFolder(user_autostart_path) == false)
            {
                return false;
            }

            std::ofstream outfile(user_autostart_path + "/pm.desktop");
            outfile << "[Desktop Entry]" << std::endl;
            outfile << "Type=Application" << std::endl;
            outfile << "Exec=" << exe_full_path_ << std::endl;
            outfile << "Hidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true" << std::endl;
            outfile << "Name[en_US]=" << exe_full_path_ << std::endl;
            outfile << "Name=" << exe_full_path_ << std::endl;
            outfile << "Comment[en_US]=\nComment=" << std::endl;
            outfile.close();
            success = true;
        #endif
        return success;
    }

    bool StartUpProgram::IsRegistered()
    {
        bool success = true;
        #ifdef _WIN32

            auto w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());

            HKEY h_key = nullptr;
            LONG result = 0;
            DWORD dw_reg_type = REG_SZ;
            std::wstring path_to_exe;
            DWORD dw_size = sizeof(path_to_exe);

            path_to_exe.resize(MAX_PATH);

            result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &h_key);

            success = (result == 0);

            if (success)
            {
                result = RegGetValueW(h_key, nullptr, L"ProcessMonitoring", RRF_RT_REG_SZ, &dw_reg_type, &path_to_exe[0], &dw_size);
                success = (result == 0);
            }
            if (success == true)
            {
                success = (wcscmp(&path_to_exe[0], &w_exe_full_path[0]) == 0);
            }

            if (h_key != nullptr)
            {
                RegCloseKey(h_key);
                h_key = nullptr;
            }

        #elif __linux__

            std::string user_autostart_path = GetCurrentUserPath() + "/.config/autostart";

            DIR* dir = opendir(user_autostart_path.data());
            if (!dir) {
                return false;
            }
            
            success = false;
            dirent* entry;
            
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) 
                {
                    std::vector<char> ans = File(user_autostart_path + "/" + entry->d_name).ReadAll();
                    std::string s(ans.begin(), ans.end());
                    if (s.find(exe_full_path_) != std::string::npos)
                    {
                        success = true;
                        break;
                    }
                }
            }
            closedir(dir);

        #endif
        return success;

    }

}