/**
 * @file optparser.cpp
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Implementation of argument parser
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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
    // https://stackoverflow.com/a/25969006
    // https://stackoverflow.com/a/31791657
    std::regex ip_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/(?:[0-9]|[12][0-9]|30)$");

    for (int i = 0; i < prefixes_count; i++)
    {
        if(!std::regex_match(ip_prefixes_array[i], ip_regex)){
            throw std::invalid_argument("Prefix " + std::string(ip_prefixes_array[i]) + " is not in right form!");
        }
        _ip_prefixes.insert(ip_prefixes_array[i]);
    }

    if(_ip_prefixes.size() == 0){
        throw std::invalid_argument("At least 1 network prefix must be given!");
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
