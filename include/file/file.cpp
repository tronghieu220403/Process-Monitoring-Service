
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "file.h"
#else
#include "include/file/file.h"
#endif


// how to ?
namespace pm
{
    File::File(std::string file_path):
        file_path_(file_path)
    {

    }

    bool File::Set(std::string file_path)
    {
        file_path_ = file_path;
    }

    std::vector<char> File::ReadAll()
    {
        // read all content of a file and return the content of that file as std::vector<char> in C++?
        std::ifstream file(file_path_);;

        // Read the file contents into a vector of characters
        std::vector<char> contents;
        char c;
        while (file.get(c)) {
            contents.push_back(c);
        }

        // Close the file
        file.close();

        // Return the vector of characters
        return contents;
    }

}