#include <iostream>
#include <getopt.h>
#include <stdlib.h>

using namespace std;

void print_usage(ostream& output, int exit_code)
{
    output << "Here will be the help message" << endl;
    exit(exit_code);
}

int main(int argc, char* argv[])
{
    const char* const short_options = "hp:r:c:";
    const struct option long_options[] = {
        {"help", 0, nullptr, 'h'},
        {"port", 1, nullptr, 'p'},
        {"root", 1, nullptr, 'r'},
        {"clients", 1, nullptr, 'c'},
        {nullptr, 0, nullptr, 0},
    };

    const char* program_name = argv[0];
    const char* port = nullptr;
    const char* root = nullptr;
    const char* clients = nullptr;

    int next_option;
    do {
        next_option = getopt_long(argc, argv, short_options, long_options, nullptr);

        switch (next_option) {
        case 'h': // -h or --help
            print_usage(cout, EXIT_SUCCESS);
            break;
        case 'p': // -p or --port
            port = optarg;
            break;
        case 'r': // -r or --root
            root = optarg;
            break;
        case 'c': // -c or --clients
            clients = optarg;
            break;
        case '?': // A user specified an invalid option
            print_usage(cerr, EXIT_FAILURE);
            break;
        case -1: // Done with options
            break;
        default: // Unexpected
            abort();
        }
    } while(next_option != -1);

    // Debug
    cout << program_name << ' ' << port << ' ' << root << ' ' << clients << endl;


    return EXIT_SUCCESS;
}
