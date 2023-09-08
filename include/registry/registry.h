#ifdef _WIN32

#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_REGISTRY_REGISTRY_H_
#define PROCESSMONITORING_REGISTRY_REGISTRY_H_

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
    class Registry
    {
    private:
        std::string registry_path_;
        const HKEY h_key_root_ = HKEY_CURRENT_USER;
        HKEY h_key_ = nullptr;
    public:
        Registry();
        Registry(const std::string& registry_path);
        bool SetRegistryPath(const std::string& registry_path);
        bool CreateBinaryValue(const std::string& name, const std::vector<char> value);
        bool DeleteContent();
        std::vector<char> GetValueArray(const std::string& key_name);
        std::vector< std::pair< std::string, std::vector<char> > > GetAllBinaryValues(); 
    };
}

#endif

#endif