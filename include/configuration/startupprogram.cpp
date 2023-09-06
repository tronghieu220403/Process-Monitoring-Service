
#ifdef _VISUAL_STUDIO_WORKSPACE
    #include "startupprogram.h"
#else
    #include "include/configuration/startupprogram.h"
#endif


namespace pm
{
    StartUpProgram::StartUpProgram(const std::string& exe_full_path) : exe_full_path_(exe_full_path)
    {
    }

    void StartUpProgram::SetThisProgram()
    {
        #ifdef _WIN32
            auto w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());
            exe_full_path_.resize(MAX_PATH);
            GetModuleFileNameW(nullptr, &w_exe_full_path[0], MAX_PATH);
        #elif __linux__

        #endif
    }

    void StartUpProgram::SetProgram(const std::string_view& exe_full_path)
    {
        exe_full_path_ = exe_full_path;
    }

    bool StartUpProgram::Register()
    {        
        #ifdef _WIN32

            auto w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());

            long result = 0;
            bool success = false;
            
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

        #endif

    }

    bool StartUpProgram::IsRegistered()
    {
        #ifdef _WIN32

            auto w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());

            HKEY h_key = nullptr;
            LONG result = 0;
            BOOL success = TRUE;
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

            return success;
        #elif __linux__

        #endif

    }

}