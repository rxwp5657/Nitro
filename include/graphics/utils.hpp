#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <cstring>
#include <fstream>

namespace nitro
{
    namespace utils
    {
        enum class StatusCode
        {
            BAD_VERTEX_SHADER,
            BAD_FRAGMENT_SHADER,
            BAD_GEOMETRY_SHADER,
            BAD_LINKAGE,
            OK
        };

        struct StatusInfo
        {
            StatusCode status_code;
            char message[512];
        };

        std::ifstream Open(std::string file_name, std::ios_base::openmode mode = std::ios_base::in);
    }
}

#endif