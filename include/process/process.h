#ifndef PROCESSMONITORING_PROCESS_PROCESS_H_
#define PROCESSMONITORING_PROCESS_PROCESS_H_

#define UNICODE
#define _UNICODE

#include <string.h>
#if defined(_WIN32)
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
#elif __linux__
	#include <sys/wait.h>
	#include <dirent.h>
#endif

#include <string>
#include <fstream>

namespace pm
{
	class Process {
	protected:
		int pid_ = 0;
		#ifdef _WIN32
			std::wstring name_;
		#elif __linux__
			std::string name_;
		#endif

	public:
		
		Process();
		//explicit Process(int id);
		explicit Process(const std::string_view& name);
	
		#ifdef _WIN32
			explicit Process(const std::wstring_view& wname);
		#endif

		#ifdef _WIN32
			std::wstring GetName() const;
		#elif __linux__
			std::string GetName() const;
		#endif
		virtual int GetPid() const;

		static int FindProcessIdByName(const std::string_view& name);
		#ifdef _WIN32
			static int FindProcessIdByName(const std::wstring_view& name);
		#endif
	};
}

#endif