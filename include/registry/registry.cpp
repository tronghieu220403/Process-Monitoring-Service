#ifdef _WIN32

#include "registry.h"

namespace pm
{

    Registry::Registry() = default;

    Registry::Registry(HKEY h_key_root, const std::string& registry_path)
    {
        SetHkeyRoot(h_key_root);
        SetRegistryPath(registry_path);
    }

    bool Registry::SetRegistryPath(const std::string& registry_path)
    {
        registry_path_ = registry_path;

        if (std::wstring w_registry_path(registry_path.begin(), registry_path.end()); RegCreateKeyExW(h_key_root_, &w_registry_path[0], static_cast<DWORD>(NULL), nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ | DELETE | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE |
    KEY_WOW64_64KEY, nullptr, &h_key_, nullptr) == ERROR_SUCCESS)
        {
            return true;
        }
        h_key_ = nullptr;
        return false;
    }

    void Registry::SetHkeyRoot(const HKEY h_key_root)
    {
        h_key_root_ = h_key_root;
    }

    bool Registry::CreateBinaryValue(const std::string& value_name, const std::vector<char> data)
    {   
        if (h_key_ == nullptr)
        {
            return 0;
        }

        if (RegSetValueExA(h_key_, &value_name[0], static_cast<DWORD>(NULL), REG_BINARY, &(std::vector<unsigned char>(data.begin(), data.end()))[0], data.size()) == ERROR_SUCCESS)
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
        DWORD ret_code;
        DWORD cch_value;
        std::string name;
        std::vector<char> bin_arr;
        DWORD bin_arr_sz;

        if (h_key_ == nullptr)
        {
            return key_arr;
        }

        ret_code = RegQueryInfoKeyW(h_key_, NULL, NULL, NULL, NULL, NULL, NULL, &c_values, &cch_max_value, &cb_max_valuedata, NULL, NULL);

        if (c_values)
        {
            //printf( "\nNumber of values: %d\n", c_values);

            for (int i = 0, ret_code = ERROR_SUCCESS; i < c_values; i++) 
            { 
                cch_value = cch_max_value + 1; 
                name.clear();
                name.resize(cch_max_value);
                ret_code = RegEnumValueA(h_key_, i, name.data(), &cch_value, NULL, NULL, NULL, NULL);
                name.resize(strlen(&name[0]));
                if (ret_code == ERROR_SUCCESS) 
                {
                    bin_arr.clear();
                    bin_arr.resize(32);
                    bin_arr_sz = 32;
                    if (RegGetValueA(h_key_, NULL, name.data(), RRF_RT_REG_BINARY, NULL, bin_arr.data(), &bin_arr_sz) == ERROR_SUCCESS)
                    {
                        bin_arr.resize(bin_arr_sz);
                        key_arr.push_back(make_pair(name, bin_arr));
                    }
                } 
            }
        }
        
        return key_arr;
    }

    void Registry::Close()
    {
        if (h_key_ != nullptr)
        {
            RegCloseKey(h_key_);
            h_key_ = nullptr;
        }
    }

    Registry::~Registry()
    {
        Close();
    }
}

#endif
