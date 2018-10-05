#include "ProcessOptions.hpp"
#include <cstdlib>
#include <iostream>
#include <getopt.h>

ProcessOptions::ProcessOptions(int argc, char **argv)
{
    const char *const short_opts = "hi:p:f:";
    const option long_opts[] = {
         { "help",       0, nullptr, 'h' },  
         { "ip",         0, nullptr, 'i' },
         { "port",       1, nullptr, 'p' },
         { "file",       0, nullptr, 'f' },
         { nullptr,      0, nullptr,  0  }
    };  

    while (true)
    {
        if(argc < 2)
            printHelp();

        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'p':
            _options["port"] = std::string(optarg);
            break;

        case 'i':
            _options["ip"] = std::string(optarg);
            break;

        case 'f':
            _options["file"] = std::string(optarg);
            break;

        case 'h':
        case '?': // Unrecognized option
        default:
            printHelp();
            break;
        }
    }
}

void ProcessOptions::printHelp()
{
    std::cout << "  -h  --help     Display this usage information.\n"
                 "  -f  --file     Persisted file name.\n"
                 "  -i  --ip       Set IP address.\n"
                 "  -p  --port     Set port.\n";
    std::exit(1);
}
