#define UNICODE
#define _UNICODE

#include "process/processcontrol.h"
namespace pm
{

	ProcessControl::ProcessControl()
    {

    };


	ProcessControl::ProcessControl(const std::string_view& name) 
	{
        pid_ = FindProcessIdByName(name);
		name_ = std::wstring(name.begin(), name.end());
		SetHandle(pid_);
	};

#ifdef _WIN32
	ProcessControl::ProcessControl(const std::wstring_view& name)
	{
        pid_ = FindProcessIdByName(name);
		name_ = static_cast<std::wstring_view>(name_);
		SetHandle(pid_);
	};
#endif

	bool ProcessControl::SetHandle(int pid)
	{
		#ifdef _WIN32
			process_handle_ = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid_);
			if (process_handle_ == NULL)
			{
				return false;
			}
			p_info_ = ProcessInfo(process_handle_);
			return true;
		#else

		#endif
	}

	bool ProcessControl::SetHandle(const std::string_view& name){
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	};

#ifdef _WIN32
	bool ProcessControl::SetHandle(const std::wstring_view& name)
	{
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	}
#endif

	bool ProcessControl::IsExists()
	{
		#ifdef _WIN32
			if (GetProcessId(process_handle_) == pid_ && pid_ == 0)
			{
				return true;
			}
			return false;
		#else

		#endif
	}

	int ProcessControl::GetPid() const
	{
        #ifdef _WIN32
            return GetProcessId(process_handle_);
        #else
		    return FindProcessIdByName(name_);
        #endif
	}

	ProcessInfo ProcessControl::GetProcessInfo()
	{
		if (IsExists())
		{
			return p_info_;
		}
		return ProcessInfo();
	}

    bool ProcessControl::TryFindHandle()
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
        #else

        #endif
    };

	ProcessControl::~ProcessControl()
	{
		#ifdef _WIN32
			CloseHandle(process_handle_);
		#else

		#endif
	}
}