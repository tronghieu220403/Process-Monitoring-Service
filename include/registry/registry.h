#ifdef _WIN32

#ifndef PROCESSMONITORING_REGISTRY_REGISTRY_H_
#define PROCESSMONITORING_REGISTRY_REGISTRY_H_

#include "ulti/everything.h"

namespace pm
{
    class Registry
    {
    private:
        std::string registry_path_;
        HKEY h_key_root_ = nullptr;
        HKEY h_key_ = nullptr;
    public:
        Registry();
        Registry(HKEY h_key_root, const std::string& registry_path);

        bool SetRegistryPath(const std::string& registry_path);
        void SetHkeyRoot(const HKEY h_key_root);

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