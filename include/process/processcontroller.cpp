#define UNICODE
#define _UNICODE

#include "include/process/processcontroller.h"

namespace pm
{

	ProcessController::ProcessController(const std::string_view& name) 
	{
        pid_ = FindProcessIdByName(name);
		name_ = std::wstring(name.begin(), name.end());
		SetHandle(pid_);
	};

#ifdef _WIN32
	ProcessController::ProcessController(const std::wstring_view& name)
	{
        pid_ = FindProcessIdByName(name);
		name_ = static_cast<std::wstring_view>(name_);
		SetHandle(pid_);
	};
#endif

	bool ProcessController::SetHandle(int pid)
	{
		#ifdef _WIN32
			process_handle_ = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid_);
			if (process_handle_ == NULL)
			{
				return false;
			}
			p_info_ = ProcessInfo(process_handle_);
			return true;
		#elif __linux__

		#endif
	}

	bool ProcessController::SetHandle(const std::string_view& name){
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	};

#ifdef _WIN32
	bool ProcessController::SetHandle(const std::wstring_view& name)
	{
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	}
#endif

	bool ProcessController::IsExists()
	{
		#ifdef _WIN32
			if (GetProcessId(process_handle_) == pid_ && pid_ == 0)
			{
				return true;
			}
			return false;
		#elif __linux__

		#endif
	}

	int ProcessController::GetPid() const
	{
        #ifdef _WIN32
            return GetProcessId(process_handle_);
        #elif __linux__
		    return FindProcessIdByName(name_);
        #endif
	}

	ProcessInfo ProcessController::GetProcessInfo()
	{
		if (IsExists())
		{
			return p_info_;
		}
		return ProcessInfo();
	}

    bool ProcessController::TryFindHandle()
    {
        #ifdef _WIN32
            if (IsExists())
            {
                return true;
            }
            else 
            {
                return SetHandle(name_);
            }
        #elif __linux__

        #endif
    };

	void ProcessController::Close()
	{
		#ifdef _WIN32
		if (process_handle_ != NULL)
		{
			CloseHandle(process_handle_);
			process_handle_ = NULL;
		}
		#elif __linux__

		#endif
	}

	ProcessController::~ProcessController()
	{
		Close();
	}
}