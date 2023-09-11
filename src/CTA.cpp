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
        bool ReadConfig();
        bool 
    };
}

#endif
