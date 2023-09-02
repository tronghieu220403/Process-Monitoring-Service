#ifndef PROCESSMONITORING_PROCESS_PROCESS_H_
#define PROCESSMONITORING_PROCESS_PROCESS_H_

#define UNICODE
#define _UNICODE

#include <string.h>
#if defined(_WIN32)
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
#else
	#include <sys/wait.h>
	#include <dirent.h>
#endif

#include <string>
#include <fstream>

#include <process/processinfo.h>

namespace pm
{
	class Process {
	private:
		int pid_;
		#ifdef _WIN32
			std::wstring name_;
		#else
			std::string name_;
		#endif
		ProcessInfo p_info_;

		#ifdef _WIN32
			HANDLE process_handle_;
		#else

		#endif

		bool SetHandle(int pid);
		bool SetHandle(const std::string_view& name);
		#ifdef _WIN32
			bool SetHandle(const std::wstring_view& name);
		#else

		#endif

	public:
		
		Process();
		explicit Process(int id);
		explicit Process(const std::string_view& name);
	
		#ifdef _WIN32
			explicit Process(const std::wstring_view& wname);
		#endif

		int GetPid() const;
		bool IsExists();

		bool TryFindHandle();

		ProcessInfo GetProcessInfo();

		static int FindProcessIdByName(const std::string_view& name);
		#ifdef _WIN32
			static int FindProcessIdByName(const std::wstring_view& name);
		#endif
		~Process();
	};
}

#endif