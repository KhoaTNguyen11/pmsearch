#include <iostream>
#include <string>
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

    // parses the filenames
    for (int i = target_str_index + 1; i < argc; i++) {
        cout << "File to search in: " << argv[i] << endl;
    }

    return EXIT_SUCCESS;
}