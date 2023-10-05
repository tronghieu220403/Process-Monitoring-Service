#include "consumer.h"
#include <algorithm>
#include <chrono>
#include <fstream>
namespace pm
{
	KernelConsumer::KernelConsumer()
	{
        network_mutex_.SetMutex("network_mutex");
        disk_io_mutex_.SetMutex("disk_io_mutex");
        trace_.LogFileName = NULL;
        trace_.LoggerName = (LPWSTR)KERNEL_LOGGER_NAME;
        trace_.BufferCallback = (PEVENT_TRACE_BUFFER_CALLBACK)(ProcessBuffer);
        trace_.EventCallback = (PEVENT_CALLBACK)(ProcessEvent);
        trace_.ProcessTraceMode = PROCESS_TRACE_MODE_REAL_TIME | PROCESS_TRACE_MODE_RAW_TIMESTAMP;// | PROCESS_TRACE_MODE_EVENT_RECORD; // create real time sesion + event should be represented as EVENT_RECORD structure

	}

    DWORD KernelConsumer::Open()
    {
        handle_trace_ = OpenTrace(&trace_);
        TRACE_LOGFILE_HEADER* p_header_ = &trace_.LogfileHeader;
        if ((TRACEHANDLE)INVALID_HANDLE_VALUE == handle_trace_)
        {
            Close();
            return GetLastError();
        }

        SetPointerSize(p_header_->PointerSize);

        if (p_header_->PointerSize != sizeof(PVOID))
        {
            p_header_ = (PTRACE_LOGFILE_HEADER)((PUCHAR)p_header_ +
                2 * (p_header_->PointerSize - sizeof(PVOID)));
        }

        return ERROR_SUCCESS;
    }

    DWORD KernelConsumer::Process()
    {
        ULONG status = ProcessTrace(&handle_trace_, 1, 0, 0);
        if (status != ERROR_SUCCESS)
        {
            Close();
        }
        return status;
    }

    void KernelConsumer::SetPointerSize(int pointer_size)
    {
        pointer_size_ = pointer_size;
    }

    int KernelConsumer::GetPointerSize()
    {
        return pointer_size_;
    }

    void KernelConsumer::SetDiskIoSharedVector(std::vector<IoInfo>& disk_io)
    {
        disk_io_vector_ = disk_io;
    }

    std::vector<IoInfo> KernelConsumer::GetDiskIoSharedVector()
    {
        disk_io_mutex_.Lock();
        std::vector<IoInfo> output = disk_io_vector_;
        disk_io_vector_.clear();
        disk_io_mutex_.Unlock();
        return output;
    }

    void KernelConsumer::SetNetworkIoSharedVector(std::vector<IoInfo>& network_io)
    {
        network_io_vector_ = network_io;
    }

    std::vector<IoInfo> KernelConsumer::GetNetworkIoSharedVector()
    {
        network_mutex_.Lock();
        std::vector<IoInfo> output = network_io_vector_;
        network_io_vector_.clear();
        network_mutex_.Unlock();
        return output;
    }

    ULONG WINAPI KernelConsumer::ProcessBuffer(PEVENT_TRACE_LOGFILE p_buffer)
    {
        UNREFERENCED_PARAMETER(p_buffer);

        return TRUE;
    }

    inline bool wstrcmp(const std::wstring& a, const std::wstring& b)
    {
        if (a.size() != b.size())
        {
            return false;
        }
        for (int i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }
        return true;
    }

    VOID WINAPI KernelConsumer::ProcessEvent(PEVENT_TRACE p_event)
    {

        if (IsEqualGUID(p_event->Header.Guid, EventTraceGuid) &&
            p_event->Header.Class.Type == EVENT_TRACE_TYPE_INFO)
        {
            ; // Skip this event.
        }
        else
        {
            Event event(p_event);
            
            int type = event.GetType();
            std::wstring guid = event.GetGuid();

            std::transform(guid.begin(), guid.end(), guid.begin(), ::toupper);

            if (guid == EventType::kThread)
            {
                if (type == 1 || type == 2 || type == 3 || type == 4)
                {
                    ProcessThread(event);
                }
            }
            else if (guid == EventType::kDiskIo)
            {
                if (type == 10 || type == 11)
                {
                    ProcessDiskIo(event);
                }
            }
            else if ((guid == EventType::kTcpip) || (guid == EventType::kUdpip))
            {
                if (type == 10 || type == 11 || type == 26 || type == 27)
                {
                    ProcessNetwork(event);
                }
            }
            
        }
                
        return;
    }

    VOID WINAPI KernelConsumer::ProcessThread(Event event)
    {
        ThreadEvent thread_event(event, GetPointerSize());

        // remember that this thread event has 2 event: start and end thread

        int type = thread_event.GetType();
        int thread_id = thread_event.GetThreadId();
        int pid = thread_event.GetProcessId();

        if (thread_id + 1 >= thread_.size())
        {
            thread_.resize(thread_id + 1);
        }
        
        // Thread start
        if (thread_event.GetType() == 1 || thread_event.GetType() == 3)
        {
            // push data to a shared pointer pointer vector
            thread_[thread_id] = pid;
            /*
            WriteDebug(std::to_string(pid) + " " + std::to_string(thread_id) + " create.");
            */
        }
        // Thread end
        else 
        {
            // remove data from a shared pointer vector
            thread_[thread_id] = 0;
            /*
            WriteDebug(std::to_string(pid) + " " + std::to_string(thread_id) + " end.");
            */
        }

        return VOID();
    }

    VOID WINAPI KernelConsumer::ProcessDiskIo(Event event)
    {
        DiskIoEvent disk_io_event(event, GetPointerSize());
        // // std::osyncstream(std::cout) << diskio_event.GetTransferSize() << std::endl;
        struct IoInfo io;
        if (disk_io_event.GetThreadId() > thread_.size() || thread_[disk_io_event.GetThreadId()] == 0)
        {
            return;
        }
        io.pid = thread_[disk_io_event.GetThreadId()];
        io.size = disk_io_event.GetTransferSize();
        io.filetime = disk_io_event.GetFileTime();

        disk_io_mutex_.Lock();
        disk_io_vector_.push_back(io);
        disk_io_mutex_.Unlock();

        return VOID();
    }

    VOID WINAPI KernelConsumer::ProcessNetwork(Event event)
    {
        NetworkEvent net_event(event, GetPointerSize());

        struct IoInfo io;
        io.pid = net_event.GetProcessId();
        io.size = net_event.GetTransferSize();
        io.filetime = net_event.GetFileTime();
        
        network_mutex_.Lock();
        network_io_vector_.push_back(io);
        network_mutex_.Unlock();
        return VOID();
    }
    
    ULONG KernelConsumer::Close()
    {
        ULONG status = ERROR_SUCCESS;
        if ((TRACEHANDLE)INVALID_HANDLE_VALUE != handle_trace_)
        {
            status = CloseTrace(handle_trace_);
            handle_trace_ = NULL;
        }
        return status;
    }

    KernelConsumer::~KernelConsumer()
    {
        if (Close() != ERROR_SUCCESS){
            // throw some exception here;
        }
        return;
    }
}