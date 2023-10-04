
#include "ulti/everything.h"

#include "etw/wmieventclass.h"
#include "etw/event.h"
#include "etw/consumer.h"

void SetUpComsumer()
{
    std::cout << "OKE" << std::endl;
    /*
    pm::KernelConsumer kc;
    if (kc.Open() != ERROR_SUCCESS)
    {
        std::cout << "Can not open trace Kernel Comsumer" << std::endl;
        return;
    }
    if (kc.Process() != ERROR_SUCCESS)
    {
        std::cout << "Can not process trace Kernel Comsummer" << std::endl;
    }
    */
    return;
}

int main()
{
    std::cout << "Hello World!\n";
    std::jthread comsume_thread(&SetUpComsumer);
    comsume_thread.join();
}

