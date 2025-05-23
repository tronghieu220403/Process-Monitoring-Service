
#include "file.h"

namespace pm
{
    File::File(std::string file_path):
        file_path_(file_path)
    {

    }

    void File::Set(std::string file_path)
    {
        file_path_ = file_path;
    }

    std::vector<char> File::ReadAll()
    {
        // read all content of a file and return the content of that file as std::vector<char> in C++?
        std::ifstream file(file_path_);

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
    
    void File::Append(const std::vector<char>& chr)
    {
        std::ofstream file;
        file.open(file_path_, std::ios_base::binary | std::ios_base::app);
        for (auto c: chr)
        {
            file.put(c);
        }
        file.close();
    }       

    void File::AppendFromFile(const std::string& src_file_path)
    {
        //std::vector<char> contents = File(src_file_path).ReadAll();
        std::ifstream input(src_file_path, std::ios::binary );
        std::ofstream output(file_path_, std::ios::binary | std::ios_base::app);

        std::copy( 
            std::istreambuf_iterator<char>(input), 
            std::istreambuf_iterator<char>( ),
            std::ostreambuf_iterator<char>(output));
        input.close();
        output.close();
    }

    bool File::SelfDelete()
    {
        try {
            if (std::filesystem::remove(file_path_))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        catch(const std::filesystem::filesystem_error& err) {
            return false;
        }

    }
}