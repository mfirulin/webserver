#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>

using namespace std;

const char* program_name;
const char* const short_options = "a:d:hp:";
const struct option long_options[] = {
    {"address", 1, nullptr, 'a'},
    {"directory", 1, nullptr, 'd'},
    {"help", 0, nullptr, 'h'},
    {"port", 1, nullptr, 'p'},
    {nullptr, 0, nullptr, 0}
};

void print_usage(ostream& out, int exit_code)
{
    out << "Usage: " << program_name << " <options>\n"
        << "    -a    --address      Address to listen\n"
        << "    -d    --directory    Directory for http resources\n"
        << "    -h    --help         Print this information\n"
        << "    -p    --port         Port to listen\n"
        << endl;
    exit(exit_code);
}

int main(int argc, char* argv[])
{
    program_name = argv[0];
    unsigned short port = 0;
    const char* dir = nullptr;
    struct in_addr local_address;
    local_address.s_addr = INADDR_ANY;

    int next_option;
    do {
        next_option = getopt_long(argc, argv, short_options, long_options, nullptr);

        switch (next_option) {

        case 'a': // -a or --address
            struct hostent* local_host_name;
            local_host_name = gethostbyname(optarg);

            if(local_host_name && local_host_name->h_length > 0)
                local_address.s_addr = *(reinterpret_cast<int*>(local_host_name->h_addr_list[0]));
            else
                cerr << "invalid host name: " << optarg << endl;
            break;

        case 'd': // -d or --directory
            dir = optarg;
            break;

        case 'h': // -h or --help
            print_usage(cout, EXIT_SUCCESS);
            break;

        case 'p': // -p or --port
            long value;
            char* end;
            value = strtol(optarg, &end, 10);
            if(*end != '\0')
                print_usage(cerr, EXIT_FAILURE);
            port = static_cast<unsigned short>(htons(value));
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

    if(optind != argc)
        print_usage(cerr, EXIT_FAILURE);


    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("webserver", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Program started");
    closelog();

    return EXIT_SUCCESS;
}
