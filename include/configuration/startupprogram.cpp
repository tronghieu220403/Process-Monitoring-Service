
#ifdef _VISUAL_STUDIO_WORKSPACE
    #include "startupprogram.h"
#else
    #include "include/configuration/startupprogram.h"
#endif


namespace pm
{
    StartUpProgram::StartUpProgram(std::string exe_full_path)
    {
        exe_full_path_ = exe_full_path;
    }

    void StartUpProgram::SetThisProgram()
    {
        #ifdef _WIN32
            std::wstring w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());
            exe_full_path_.resize(MAX_PATH);
            GetModuleFileNameW(NULL, &w_exe_full_path[0], MAX_PATH);
        #elif __linux__

        #endif
    }

    void StartUpProgram::SetProgram(std::string exe_full_path)
    {
        exe_full_path_ = exe_full_path;
    }

    bool StartUpProgram::Register()
    {        
        #ifdef _WIN32

            std::wstring w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());

            long result = 0;
            bool success = 0;
            
            HKEY h_key = NULL;
            result = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &h_key, NULL);
            
            success = (result == 0);
            
            if (success)
            {
                result = RegSetValueExW(h_key, L"ProcessMonitoring",0, REG_SZ, (BYTE *)&w_exe_full_path[0], (wcslen(&w_exe_full_path[0]) + 1) * 2);
                success = (result == 0);
            }

            if (h_key != NULL)
            {
                RegCloseKey(h_key);
                h_key = NULL;
            }

            return success;
        #elif __linux__

        #endif

    }

    bool StartUpProgram::IsRegistered()
    {
        #ifdef _WIN32

            std::wstring w_exe_full_path = std::wstring(exe_full_path_.begin(), exe_full_path_.end());

            HKEY h_key = NULL;
            LONG result = 0;
            BOOL success = TRUE;
            DWORD dw_reg_type = REG_SZ;
            wchar_t path_to_exe[MAX_PATH]  = {};
            DWORD dw_size = sizeof(path_to_exe);

            result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &h_key);

            success = (result == 0);

            if (success)
            {
                result = RegGetValueW(h_key, NULL, L"ProcessMonitoring", RRF_RT_REG_SZ, &dw_reg_type, &path_to_exe[0], &dw_size);
                success = (result == 0);
            }
            
            success = (wcscmp(path_to_exe, &w_exe_full_path[0]) == 0);

            if (h_key != NULL)
            {
                RegCloseKey(h_key);
                h_key = NULL;
            }

            return success;
        #elif __linux__

        #endif

    }

}