#include "provider.h"

namespace pm
{

    KernelProvider::KernelProvider()
    {
        ZeroMemory(&session_properties_, sizeof(session_properties_));
        session_properties_.Wnode.BufferSize = sizeof(session_properties_);
        session_properties_.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
        session_properties_.Wnode.ClientContext = 1; //QPC clock resolution
        session_properties_.Wnode.Guid = SystemTraceControlGuid;
        session_properties_.LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
    }

    KernelProvider::KernelProvider(ULONG flags)
    {
        KernelProvider();
        SetFlags(flags);
    }

    void KernelProvider::SetFlags(ULONG flags)
    {
        session_properties_.EnableFlags = flags;
    }


    TRACEHANDLE KernelProvider::GetSessionHandle() const {
        return session_handle_;
    }

    void KernelProvider::SetSessionHandle(TRACEHANDLE session_handle) {
        session_handle_ = session_handle;
    }

    EVENT_TRACE_PROPERTIES KernelProvider::GetSessionProperties() const {
        return session_properties_;
    }


    void KernelProvider::SetSessionProperties(EVENT_TRACE_PROPERTIES p_session_properties) {
        session_properties_ = p_session_properties;
    }



    ULONG KernelProvider::BeginTrace()
	{
        ULONG status = ERROR_SUCCESS;

        status = StartTrace((PTRACEHANDLE)&session_handle_, KERNEL_LOGGER_NAME, &session_properties_);

        return status;

	}

    ULONG KernelProvider::CloseTrace()
	{
        ULONG status = ERROR_SUCCESS;
        if (session_handle_ != NULL)
        {
            status = ControlTrace(session_handle_, KERNEL_LOGGER_NAME, &session_properties_, EVENT_TRACE_CONTROL_STOP);
            session_handle_ = NULL;
        }

        session_properties_ = { 0 };
        return status;
	}
}