#include "processcontroller.h"

namespace pm
{

	ProcessController::ProcessController(const std::string& name)
	{
		this->Process::SetName(name);
        SetHandle();
	}

	ProcessController::ProcessController(const ProcessController& pc):p_info_(pc.p_info_)
	{
		this->Process::SetName(pc.GetName());
        SetHandle();
	}

	ProcessController& ProcessController::operator=(const ProcessController& pc)
	{
		this->p_info_ = pc.p_info_;
		this->Process::SetName(pc.GetName());
		this->SetHandle();
		return *this;
	}

	bool ProcessController::SetHandle()
	{
		std::string name = Process::GetName();
		Process::SetPid(FindProcessIdByName(name));
		int pid = Process::GetPid();
		if (pid == 0)
		{
			return false;
		}

		bool success = true;
		#ifdef _WIN32
			process_handle_ = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
			if (process_handle_ == nullptr)
			{
				Process::SetPid(0);
				success = false;
			}
			else 
			{
				p_info_ = std::make_shared<ProcessInfo>(name, pid);
				success = true;
			}
		#elif __linux__

            if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
            {
                return false;
            }

			p_info_ = std::make_shared<ProcessInfo>(name, pid);

		#endif
		
		return success;

	}

	bool ProcessController::IsExists()
	{
		#ifdef _WIN32
			int pid = GetPid();
			if (process_handle_ != (HANDLE)-1 && process_handle_ != 0 && GetProcessId(process_handle_) == pid && pid != 0)
			{
				return true;
			}
			return false;
		#elif __linux__

            if (std::filesystem::is_directory("/proc/" + std::to_string(GetPid())) == false)
            {
                return 0;
            }

			return false;
		#endif
	}

	std::shared_ptr<ProcessInfo> ProcessController::GetProcessInfo() const
	{
		return p_info_;
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
				Close();
                return SetHandle();
            }
        #elif __linux__
			return false;

        #endif
    };

	void ProcessController::Close()
	{
		#ifdef _WIN32
		if (process_handle_ != nullptr)
		{
			CloseHandle(process_handle_);
			process_handle_ = nullptr;
		}
		#elif __linux__

		#endif
	}

	ProcessController::~ProcessController()
	{
		Close();
	}
}