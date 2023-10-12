#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex>
#include <string>
#include <stdexcept>

#include "optparser.h"

Options::~Options() = default;

Options::Options() = default;

Options::Options(int argc, char **argv)
{
    _print_help = false;
    _file_name = NULL;
    _interface_name = NULL;

    int c;

    while ((c = getopt(argc, argv, "hr:i:")) != -1)
    {
        switch (c)
        {
            case 'h':
                _print_help = true;
                break;
            case 'r':
                _file_name = optarg;
                break;
            case 'i':
                _interface_name = optarg;
                break;
            case '?':
                _print_help = true;
                return;
            default:
                break;
        }
    }

    char **ip_prefixes_array = &argv[optind];
    int prefixes_count = argc - optind;

    std::regex ip_regex("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$");

    for (int i = 0; i < prefixes_count; i++)
    {
        if(!std::regex_match(ip_prefixes_array[i], ip_regex)){
            throw std::invalid_argument("Prefix " + std::string(ip_prefixes_array[i]) + " is not in right form!");
        }
        _ip_prefixes.insert(ip_prefixes_array[i]);
    }

    if(!_file_name && !_interface_name){
        throw std::invalid_argument("Either interface name or file name must be provided!");
    }

}

char* Options::GetFileName(){
    return _file_name;
}

char* Options::GetInterfaceName(){
    return _interface_name;
}

std::set<char *> Options::GetIPPrefixes(){
    return _ip_prefixes;
}

bool Options::PrintHelp(){
    return _print_help;
}
