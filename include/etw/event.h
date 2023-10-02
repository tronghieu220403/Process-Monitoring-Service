#pragma once

#ifndef PROCESSMONITORING_ETW_EVENT_H_
#define PROCESSMONITORING_ETW_EVENT_H_

#include "ulti/everything.h"

namespace pm
{
	class Event
	{
	private:
		std::wstring guid_;
		int version_ = 0;
		int type_ = 0;
		FILETIME ft_ = { 0 };
		ULONGLONG time_stamp_ = 0;
		ULONG mof_length_ = 0;

		PBYTE p_event_data_ = NULL;

	public:
		Event();
		Event(PEVENT_TRACE p_event);

		std::wstring GetGuid() const;
		void SetGuid(std::wstring guid);

		int GetVersion() const;
		void SetVersion(int version_);

		int GetType() const;
		void SetType(int type);

		FILETIME GetFileTime() const;
		void SetFileTime(FILETIME ft);

		ULONGLONG GetTimeStamp() const;
		void SetTimeStamp(ULONGLONG time_stamp);
		ULONGLONG GetTimeInMs() const;

		ULONG GetMofLength() const;
		void SetMofLength(ULONG mof_length);

		PBYTE GetPEventData() const;
		void SetPEventData(PBYTE p_event_data);

	};

    class EventType
    {
	public: 
		inline static const std::wstring kThread = L"{3D6FA8D1-FE05-11D0-9DDA-00C04FD7BA7C}";
		inline static const std::wstring kProcess = L"{3D6FA8D0-FE05-11D0-9DDA-00C04FD7BA7C}";
		inline static const std::wstring kDiskIo = L"{3D6FA8D4-FE05-11D0-9DDA-00C04FD7BA7C}";
		inline static const std::wstring kTcpip = L"{9A280AC0-C8E0-11D1-84E2-00C04FB998A2}";
		inline static const std::wstring kUdpip = L"{BF3A50C5-A9C9-4988-A005-2DF0B7C80F80}";
    };

};

#endif
