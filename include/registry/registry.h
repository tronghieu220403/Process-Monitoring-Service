#ifdef _WIN32

#ifndef PROCESSMONITORING_REGISTRY_REGISTRY_H_
#define PROCESSMONITORING_REGISTRY_REGISTRY_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

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
        void Close();
        ~Registry();
    };
}

#endif

#endif