#ifdef _VISUAL_STUDIO_WORKSPACE
#include "process.h"
#else
#include "include/process/process.h"
#endif
#include <iostream>

namespace pm
{

	Process::Process() = default;

	Process::Process(const std::string& name): pid_(FindProcessIdByName(name)), name_(name)
	{
	};

	void Process::SetName(const std::string_view& name)
	{
		name_ = name;
	}

	void Process::SetPid(const int pid)
	{
		pid_ = pid;
	}


	std::string Process::GetName() const
	{
		return name_;
	}

	int Process::GetPid()
	{
		pid_ = FindProcessIdByName(name_);
		return pid_;
	}


#ifdef _WIN32
	int Process::FindProcessIdByName(const std::wstring_view& name)
	{
		int pid = 0;
		PROCESSENTRY32 entry{};
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

}