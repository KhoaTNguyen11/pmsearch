#include <iostream>
#include <string>

#include "MappedFile.h"
using namespace std;

string usage = "Usage: ./pmsearch [-p] <target_string> <file1> [file2 ... fileN]";

int main(int argc, char* argv[]) 
{
    bool is_parallel = false;
    string target_str;
    cout << "Hello from the Parallel MMAP Search (pmsearch) utility!" << endl;

    if (argc < 3) {
        cout << "Missing arguments!\n";
        cout << usage << endl;
        return EXIT_FAILURE;
    }

    if (string("-p") == string(argv[1])) {
        cout << "Turned on parallel mode!\n";
        is_parallel = true;
        if (argc < 4) {
            cout << "Missing arguments!\n";
            cout << usage << endl;
            return EXIT_FAILURE;
        }
    }

    // parses the target string
    int target_str_index = 1;
    if (is_parallel == true) {
        target_str_index = 2;
    }
    
    target_str = argv[target_str_index];
    cout << "Target string is: " << target_str << endl;

    // actually looking through now
    for (int i = target_str_index + 1; i < argc; i++) {
        try {
            MappedFile mf{string(argv[i])};
            auto offsets = mf.find_all(target_str);
            for (auto offset : offsets) {
                std::cout << argv[i]  << ":0x" << std::hex << std::uppercase << offset << std::endl;
            }
    
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << argv[i] << ": " << e.what() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}