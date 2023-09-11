#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_SRC_CTA_H_
#define PROCESSMONITORING_SRC_CTA_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define _CRT_SECURE_NO_DEPRECATE
#define UNICODE
#define _UNICODE
#endif

#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processinfo.h"
#include "E:/Code/Github/Process-Monitoring/include/process/processcontroller.h"
#include "E:/Code/Github/Process-Monitoring/include/logs/processlogger.h"
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#else
#include "include/process/processinfo.h"
#include "include/process/processcontroller.h"
#include "include/logs/processlogger.h"
#include "include/ulti/collections.h"


#include <iostream>
#include <string>
#include <vector>

namespace pm
{
    class Cta
    {
    private:
    public:
        Cta();
        bool UpdateConfig();
        void Monitoring();
        void CommunicateWithCtb();
    };
}

#endif
