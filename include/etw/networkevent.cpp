#include "networkevent.h"

namespace pm
{
    NetworkEvent::NetworkEvent()
    {
        
    }

    NetworkEvent::NetworkEvent(PEVENT_TRACE p_event, int pointer_size)
    {
        this->Event::Event(p_event);
        if (FindPropertiesInfo(pointer_size) == false)
        {
            return;
        }
        UpdateAttributes();
    }

    NetworkEvent::NetworkEvent(Event event, int pointer_size)
    {
        this->Event::Event(event);
        if (FindPropertiesInfo(pointer_size) == false)
        {
            return;
        }
        UpdateAttributes();
    }

    bool NetworkEvent::FindPropertiesInfo(int pointer_size)
    {
        if (GetTransferSizeOffset() == -1 || GetTransferSizeLength() == -1)
        {
            std::pair<int, int> prop_info =  WmiEventClass(EventType().kTcpip, Event::GetVersion(), Event::GetType(), pointer_size).GetPropertyInfo(L"Transfer Size in Bytes");
            if (prop_info.first != -1 && prop_info.second != 0)
            {
                SetTransferSizeOffset(prop_info.first);
                SetTransferSizeLength(prop_info.second);
            }
        }
        if (GetProcessIdOffset() == -1 || GetProcessIdLength() == -1)
        {
            std::pair<int, int> prop_info =  WmiEventClass(EventType().kTcpip, Event::GetVersion(), Event::GetType(), pointer_size).GetPropertyInfo(L"Process ID");
            if (prop_info.first != -1 && prop_info.second != 0)
            {
                SetProcessIdOffset(prop_info.first);
                SetProcessIdLength(prop_info.second);
            }
        }

        if (GetTransferSizeOffset() == -1 || GetTransferSizeLength() == -1 || GetProcessIdOffset() == -1 || GetProcessIdLength() == -1)
        {
            return false;
        }
        return true;
    }

    void NetworkEvent::UpdateAttributes()
    {
        unsigned long long transfer_size = 0;
        int pid = 0;

        memcpy(&transfer_size, this->Event::GetPEventData() + GetTransferSizeOffset(), GetTransferSizeLength());
        SetTransferSize(transfer_size);

        memcpy(&pid, this->Event::GetPEventData() + GetProcessIdOffset(), GetProcessIdLength());
        SetProcessId(pid);

    }

    void NetworkEvent::SetTransferSize(unsigned long long transfer_size)
    {
        transfer_size_ = transfer_size;
    }

    unsigned long long NetworkEvent::GetTransferSize() const
    {
        return transfer_size_;
    }

    void NetworkEvent::SetProcessId(int pid)
    {
        process_id_ = pid;
    }
    
    int NetworkEvent::GetProcessId() const
    {
        return process_id_;
    }

    void NetworkEvent::SetTransferSizeOffset(int transfer_size_offset)
    {
        transfer_size_offset_ = transfer_size_offset;

    }
    int NetworkEvent::GetTransferSizeOffset()
    {
        return transfer_size_offset_;
    }

    void NetworkEvent::SetTransferSizeLength(int transfer_size_length)
    {
        transfer_size_length_ = transfer_size_length;
    }

    int NetworkEvent::GetTransferSizeLength()
    {
        return transfer_size_length_;
    }

    void NetworkEvent::SetProcessIdOffset(int process_id_offset)
    {
        process_id_offset_ = process_id_offset;
    }

    int NetworkEvent::GetProcessIdOffset()
    {
        return process_id_offset_;
    }

    void NetworkEvent::SetProcessIdLength(int process_id_length)
    {
        process_id_length_ = process_id_length;
    }

    int NetworkEvent::GetProcessIdLength()
    {
        return process_id_length_;
    }

};
