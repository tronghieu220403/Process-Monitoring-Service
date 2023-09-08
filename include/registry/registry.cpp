#ifdef _WIN32

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "registry.h"
#else
#include "include/registry/registry.h"
#endif

namespace pm
{

    Registry::Registry() = default;

    Registry::Registry(const std::string& registry_path)
    {
        SetRegistryPath(registry_path);
    }

    bool Registry::SetRegistryPath(const std::string& registry_path)
    {
        registry_path_ = registry_path;
        std::wstring w_registry_path(registry_path.begin(), registry_path.end());

        if (RegCreateKeyExW(h_key_root_, &w_registry_path[0], static_cast<DWORD>(NULL), nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ | DELETE | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE |
    KEY_WOW64_64KEY, nullptr, &h_key_, nullptr) == ERROR_SUCCESS)
        {
            return true;
        }
        h_key_ = nullptr;
        return false;
    }

    bool Registry::CreateBinaryValue(const std::string& value_name, const std::vector<char> data)
    {   
        if (h_key_ == nullptr)
        {
            return 0;
        }

        if (RegSetValueExW(h_key_, &(std::wstring(value_name.begin(),value_name.end()))[0], static_cast<DWORD>(NULL), REG_BINARY, (const BYTE*)data[0], data.size()) == ERROR_SUCCESS)
        {
            return true;
        }

        return false;
    }
    
    bool Registry::DeleteContent()
    {
        if (h_key_ != nullptr && RegDeleteTreeW(h_key_,nullptr) == ERROR_SUCCESS)
        {
            return true;
        }
        return false;
    }

    std::vector<char> Registry::GetValueArray(const std::string& key_name)
    {
        std::vector<char> value;
        if (h_key_ == nullptr)
        {
            return value;
        }
        
        return value;
    }

    std::vector< std::pair< std::string, std::vector<char> > > Registry::GetAllBinaryValues()
    {
        std::vector< std::pair<std::string, std::vector<char>> > key_arr;

        DWORD c_values;              // number of values for key 
        DWORD cch_max_value;          // longest value name 
        DWORD cb_max_valuedata;       // longest value data 
        DWORD retCode;
        DWORD cch_value;
        std::string name;
        std::vector<char> bin_arr;
        DWORD bin_arr_sz;

        if (h_key_ == nullptr)
        {
            return key_arr;
        }

        retCode = RegQueryInfoKeyW(h_key_, NULL, NULL, NULL, NULL, NULL, NULL, &c_values, &cch_max_value, &cb_max_valuedata, NULL, NULL);

        std::cout << cch_max_value << std::endl;

        if (c_values)
        {
            printf( "\nNumber of values: %d\n", c_values);

            for (int i = 0, retCode = ERROR_SUCCESS; i < c_values; i++) 
            { 
                cch_value = cch_max_value; 
                name.clear();
                name.resize(cch_max_value);
                retCode = RegEnumValueA(h_key_, i, name.data(), &cch_value, NULL, NULL, NULL, NULL);
                name.resize(cch_value + 1);
                if (retCode == ERROR_SUCCESS) 
                {
                    bin_arr.clear();
                    bin_arr.resize(32);
                    bin_arr_sz = 32;
                    if (RegGetValueA(h_key_, NULL, name.data(), RRF_RT_REG_BINARY, NULL, bin_arr.data(), &bin_arr_sz) == ERROR_SUCCESS)
                    {
                        bin_arr.resize(bin_arr_sz);
                        key_arr.emplace_back(make_pair(name, bin_arr));
                    }
                } 
            }
        }
        
        return key_arr;
    }

}

#endif
