#include <utils.hpp>

namespace nitro
{
    namespace utils
    {
        std::ifstream Open(std::string file_name, std::ios_base::openmode mode)
        {   
            std::string rsc_path{"../resources/"};

            std::string path = rsc_path.append(file_name);
            std::ifstream file{path, mode};
            
            if(!file.is_open())
            {
                std::string error{"Unable to open file: "};
                error.append(path);
                throw std::runtime_error{ error }; 
            }
            file.exceptions(std::ifstream::badbit);
            return file;
        }
    }
}