#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include "server.h"

using namespace std;

const char* program_name;
const char* const short_options = "a:d:hp:";
const option long_options[] = {
    {"address", 1, nullptr, 'a'},
    {"directory", 1, nullptr, 'd'},
    {"help", 0, nullptr, 'h'},
    {"port", 1, nullptr, 'p'},
    {nullptr, 0, nullptr, 0}
};

void print_usage(ostream& out, int exit_code)
{
    out << "Usage: " << program_name << " [ options ]\n"
        << "    -a, --address ADDR     Address to listen\n"
        << "    -d, --directory DIR    Directory for http resources\n"
        << "    -h, --help             Print this information\n"
        << "    -p, --port PORT        Port to listen\n"
        << endl;
    exit(exit_code);
}

int main(int argc, char* argv[])
{
    program_name = argv[0];
    uint16_t port = 0;
    const char* dir = ".";
    in_addr local_address;
    local_address.s_addr = INADDR_ANY;
    Server* server = nullptr;

    // Initialize syslog. Priorities:
    // LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG.
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog("webserver", LOG_CONS | LOG_PID | LOG_NDELAY | LOG_PERROR, LOG_LOCAL0);

    int next_option;
    do {
        next_option = getopt_long(argc, argv, short_options, long_options, nullptr);

        switch (next_option) {

        case 'a': // -a or --address
            hostent* local_host_name;
            local_host_name = gethostbyname(optarg);
            if(local_host_name && local_host_name->h_length > 0)
                local_address.s_addr = *(reinterpret_cast<int*>(local_host_name->h_addr_list[0]));
            else
                syslog(LOG_WARNING, "Invalid address to listen \"%s\", use ANY", optarg);
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

    try {
        server = new Server(local_address, port, dir);
    }
    catch(bad_alloc& e) {
        syslog(LOG_CRIT, "Cannot allocate server: %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch(exception& e) {
        syslog(LOG_CRIT, "Exception occured: %s. %s", e.what(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    catch(...) {
        syslog(LOG_CRIT, "Unexpected exception occured: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    syslog(LOG_NOTICE, "Started: address=%d port=%d dir=%s", local_address.s_addr, port, dir);

    server->run();

    server->stop();

    closelog();
    return EXIT_SUCCESS;
}
