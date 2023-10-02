#pragma once

#ifndef PROCESSMONITORING_ETW_NETWORKEVENT_H_
#define PROCESSMONITORING_ETW_NETWORKEVENT_H_

#include "etw/event.h"
#include "etw/wmieventclass.h"

namespace pm
{
	class NetworkEvent: public Event
	{
	private:
	
		inline static int transfer_size_offset_ = -1;
		inline static int transfer_size_length_ = -1;

		inline static int process_id_offset_ = -1;
		inline static int process_id_length_ = -1;

		unsigned long long transfer_size_ = 0;
        int process_id_ = -1;

	public:

		NetworkEvent();
		NetworkEvent(PEVENT_TRACE p_event, int pointer_size);
		NetworkEvent(Event event, int pointer_size);

		bool FindPropertiesInfo(int pointer_size);
		void UpdateAttributes();

		void SetTransferSize(unsigned long long transfer_size);
		unsigned long long GetTransferSize() const;

		void SetProcessId(int process_id);
		int GetProcessId() const;

		static void SetTransferSizeOffset(int transfer_size_offset);
		static int GetTransferSizeOffset();
		static void SetTransferSizeLength(int transfer_size_length);
		static int GetTransferSizeLength();

		static void SetProcessIdOffset(int process_id_offset);
		static int GetProcessIdOffset();
		static void SetProcessIdLength(int process_id_length);
		static int GetProcessIdLength();


	};

};

#endif
