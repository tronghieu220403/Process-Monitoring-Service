#ifdef _WIN32

#pragma once

#ifndef PROCESSMONITORING_ETW_COMSUMER_H_
#define PROCESSMONITORING_ETW_COMSUMER_H_

#define INITGUID

#include "ulti/everything.h"
#include "ulti/collections.h"
#include "mutex/mutex.h"

#include "etw/event.h"
#include "etw/diskioevent.h"
#include "etw/networkevent.h"
#include "etw/threadevent.h"

namespace pm
{

	class KernelConsumer
	{
	private:
		EVENT_TRACE_LOGFILE trace_ = { 0 };
		TRACEHANDLE handle_trace_ = NULL;
		inline static int pointer_size_ = 0;

		inline static NamedMutex network_mutex_;
		inline static NamedMutex disk_io_mutex_;

		inline static std::vector<int> thread_;

		inline static std::vector<IoInfo> disk_io_vector_;
		inline static std::vector<IoInfo> network_io_vector_;

	public:
		KernelConsumer();

		DWORD Open();
		DWORD Process();

		static void SetPointerSize(int pointer_size);
		static int GetPointerSize();

		static void SetNetworkIoSharedVector(std::vector<IoInfo>& network_io);
		static std::vector<IoInfo> GetNetworkIoSharedVector();

		static void SetDiskIoSharedVector(std::vector<IoInfo>& disk_io);
		static std::vector<IoInfo> GetDiskIoSharedVector();

		static ULONG WINAPI ProcessBuffer(PEVENT_TRACE_LOGFILE p_buffer);
		static VOID WINAPI ProcessEvent(PEVENT_TRACE p_event);

		static VOID WINAPI
			ProcessThread(Event event);

		static VOID WINAPI
			ProcessDiskIo(Event event);

		static VOID WINAPI
			ProcessNetwork(Event event);

		ULONG Close();

		~KernelConsumer();
	};

};

#endif

#endif