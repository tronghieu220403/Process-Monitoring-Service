#pragma once

#ifndef PROCESSMONITORING_ETW_COMSUMER_
#define PROCESSMONITORING_ETW_COMSUMER_

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

		inline static std::shared_ptr< std::vector<IoInfo> > disk_io_vector_ = std::make_shared<std::vector<IoInfo>>();
		inline static std::shared_ptr< std::vector<IoInfo> > network_io_vector_ = std::make_shared<std::vector<IoInfo>>();

	public:
		KernelConsumer();

		DWORD Open();
		DWORD Process();

		static void SetPointerSize(int pointer_size);
		static int GetPointerSize();

		static void SetDiskIoSharedVector(std::shared_ptr< std::vector<IoInfo> >& disk_io);
		static std::shared_ptr<std::vector<IoInfo>> GetDiskIoSharedVector() const;

		static void SetNetworkIoSharedVector(std::shared_ptr< std::vector<IoInfo> >& network_io);
		static std::shared_ptr<std::vector<IoInfo>> GetNetworkIoSharedVector() const;

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
