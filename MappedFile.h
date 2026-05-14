#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#include <string>
#include <cstddef>
#include <vector>
using namespace std;

class MappedFile {
    public:
        MappedFile(const std::string& filename); // Constructor
        ~MappedFile();                           // Destructor

        std::vector<size_t> find_all(const std::string_view target_string);

    private:
        int fd_;                                // File descriptor
        void *data_;                            // Pointer to file data
        size_t length_;                         // Size of file in bytes
};

#endif