#define UNICODE
#define _UNICODE

#include "process/process.h"
namespace pm
{

	Process::Process(): pid_(0) {};

	Process::Process(int id): pid_(id) {};

	Process::Process(const std::string_view& name): pid_(FindProcessIdByName(name)) 
	{
		name_ = std::wstring(name.begin(), name.end());
		SetHandle(pid_);
	};

#ifdef _WIN32
	Process::Process(const std::wstring_view& name): pid_(FindProcessIdByName(name))
	{
		name_ = static_cast<std::wstring_view>(name_);
		SetHandle(pid_);
	};
#endif

	bool Process::SetHandle(int pid)
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

	int Process::GetPid() const
	{
		return FindProcessIdByName(name_);
	}

	bool Process::IsExists()
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

	ProcessInfo Process::GetProcessInfo()
	{
		if (IsExists())
		{
			return p_info_;
		}
		return ProcessInfo();
	}

	bool Process::SetHandle(const std::string_view& name){
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	};

#ifdef _WIN32
	bool Process::SetHandle(const std::wstring_view& name)
	{
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	}
#endif

bool Process::TryFindHandle(){
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


#ifdef _WIN32
	int Process::FindProcessIdByName(const std::wstring_view& name)
	{
		int pid = 0;
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (Process32FirstW(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (wcscmp(entry.szExeFile, &name[0]) == 0)
				{
					pid = entry.th32ProcessID;
					break;
				}
			}
		}
		CloseHandle(snapshot);
		return pid;
	}
#endif

	int Process::FindProcessIdByName(const std::string_view& name){
		int pid = 0;
		#ifdef _WIN32
			pid = FindProcessIdByName(std::wstring(name.begin(), name.end()));
		#elif __linux__
			DIR* dir = opendir("/proc");
			if (!dir) {
				return false;
			}

			bool found = false;
			dirent* entry;
			while ((entry = readdir(dir)) != nullptr && !found) {
				// check if the entry is a directory and its name is a number
				if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
					// read the process name from the cmdline file
					std::string cmdPath = std::string("/proc/") + entry->d_name + "/cmdline";
					//std::cout << cmdPath.c_str() << std::endl;
					
					std::ifstream cmdFile(cmdPath.c_str());
					std::string cmdLine;
					std::getline(cmdFile, cmdLine);

					// check if the process name matches
					if (!cmdLine.empty() && cmdLine.find(name) != std::string::npos) {
						pid = std::stoi(entry->d_name);
						break;
					}
					
				}
			}
			closedir(dir);

		#endif
			return pid;
	};

	Process::~Process()
	{
		#ifdef _WIN32
			CloseHandle(process_handle_);
		#else

		#endif

	}
}