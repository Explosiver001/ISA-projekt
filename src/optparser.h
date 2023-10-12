#ifndef _OPT_PARSER_H_
#define _OPT_PARSER_H_

#include <string>
#include <set>

class Options{
    private:
        char *_file_name;
        char *_interface_name;
        std::set<char *> _ip_prefixes;
        bool _print_help;
        
    public:
        ~Options();
        Options();
        Options(int argc, char **argv);
        bool PrintHelp();
        char* GetFileName();
        char* GetInterfaceName();
        std::set<char *> GetIPPrefixes();
};


#endif