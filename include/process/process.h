#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_PROCESS_PROCESS_H_
#define PROCESSMONITORING_PROCESS_PROCESS_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif


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
	private:
	
		int pid_ = 0;
		std::string name_;

	protected:
		void SetName(const std::string& name);
		void SetPid(const int pid);

	public:
		
		Process();

		explicit Process(const std::string& name);

		std::string GetName() const;
		
		virtual int GetPid();

		static int FindProcessIdByName(const std::string& name);
		#ifdef _WIN32
			static int FindProcessIdByName(const std::wstring& name);
		#endif
	};
}

#endif