#ifndef PROCESSMONITORING_PROCESS_PROCESS_H_
#define PROCESSMONITORING_PROCESS_PROCESS_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

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