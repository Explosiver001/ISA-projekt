#ifndef _OPT_PARSER_H_
#define _OPT_PARSER_H_

#include <string>

class Options{
    private:
        char *_file_name;
        char *_interface_name;
        char **_ip_prefixes;
        int _prefixes_count;
        bool _print_help;
        
    public:
        Options();
        Options(int argc, char **argv);
        char* GetFileName();
        char* GetInterfaceName();
};


#endif