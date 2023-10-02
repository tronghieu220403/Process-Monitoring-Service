#pragma once

#ifndef PROCESSMONITORING_ETW_PROVIDER_H_
#define PROCESSMONITORING_ETW_PROVIDER_H_

#define INITGUID

#include "ulti/everything.h"

namespace pm
{
	class KernelProvider
	{
	private:
		TRACEHANDLE session_handle_ = NULL;
		EVENT_TRACE_PROPERTIES session_properties_ = { 0 };

	public:

		KernelProvider();
		KernelProvider(ULONG flags);

		void SetFlags(ULONG flags);

    	TRACEHANDLE GetSessionHandle() const;
		void SetSessionHandle(TRACEHANDLE session_handle);
		EVENT_TRACE_PROPERTIES GetSessionProperties() const;
		void SetSessionProperties(EVENT_TRACE_PROPERTIES p_session_properties);

		ULONG BeginTrace();
		ULONG CloseTrace();
	};

}


#endif
