
#include "ulti/everything.h"

#include "etw/wmieventclass.h"
#include "etw/event.h"
#include "etw/consumer.h"

void SetUpComsumer();

int main()
{
    std::cout << "Hello World!\n";
    SetUpComsumer();
}

void SetUpComsumer()
{
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
}