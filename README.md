# pmsearch (Parallel MMAP Search)
A high-performance C++20 utility for searching patterns in large files using `mmap`.

## Features
- **Zero-copy:** Uses memory mapping for efficient file access.
- **Modern C++:** Implements RAII, `std::string_view`, and uniform initialization.
- **Robust:** Includes a Bash test suite for edge-case verification.

## Building
```bash
make
./pmsearch <pattern> <file1> <file2>...
