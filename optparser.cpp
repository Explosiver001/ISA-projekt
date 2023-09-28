#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "optparser.h"

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

    _ip_prefixes = &argv[optind];
    _prefixes_count = argc - optind;

    for (int i = 0; i < _prefixes_count; i++)
    {
        printf("Non-option argument %s\n", _ip_prefixes[i]);
    }

    printf("filename = %s\n", _file_name ? _file_name : "");
    printf("interfacename = %s\n", _interface_name ? _interface_name : "");
    printf("Use help: %d\n\n", _print_help);
}

char* Options::GetFileName(){
    return _file_name;
}

char* Options::GetInterfaceName(){
    return _interface_name;
}