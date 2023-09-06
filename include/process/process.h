#pragma once

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
	protected:
	
		int pid_ = 0;
		std::string name_;

	public:
		
		Process();

		explicit Process(const std::string_view& name);

		std::string GetName() const;
		
		virtual int GetPid() const;

		static int FindProcessIdByName(const std::string_view& name);
		#ifdef _WIN32
			static int FindProcessIdByName(const std::wstring_view& name);
		#endif
	};
}

#endif