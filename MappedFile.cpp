#include "MappedFile.h"

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string_view>

// constructor
MappedFile::MappedFile(const std::string& filename) 
{
    fd_ = open(filename.c_str(), O_RDONLY);
    if (fd_ < 0) {
        throw runtime_error("Could not open file");
    }

    struct stat stat_st;
    if (fstat(fd_, &stat_st) < 0) {
        throw runtime_error("Could not stat file");
    }

    length_ = stat_st.st_size;
    if (length_ == 0) {
        data_ = nullptr;
        return;
    }

    // actually doing the mapping from VA to PA 
    data_ = mmap(nullptr, length_, PROT_READ, MAP_PRIVATE, fd_, 0);
    if (data_ == MAP_FAILED) {
        close(fd_);
        throw std::runtime_error("mmap failed");
    }
}

// destructor
MappedFile::~MappedFile() 
{
    if (data_ != nullptr) {
        munmap(data_, length_);
    }

    if (fd_ >= 0) {
        close(fd_);
    }
}

// find all instances of target string in the mapped file
std::vector<size_t> MappedFile::find_all(const std::string_view target_string)
{
    std::vector<size_t> positions;
    std::string_view strv ((char* )data_, length_);

    size_t pos = strv.find(target_string, 0);

    while (pos != strv.npos) {
        positions.push_back(pos);
        pos = strv.find(target_string, pos + target_string.size());
    }

    return positions;
}