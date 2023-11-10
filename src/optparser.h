/**
 * @file optparser.h
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Declaration of argument parser
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _OPT_PARSER_H_
#define _OPT_PARSER_H_

#include <string>
#include <set>

class Options{
    private:
        char *_file_name; // file name when using -r
        char *_interface_name; // interface name when using -i 
        std::set<char *> _ip_prefixes; // all prefixes
        bool _print_help; 
        
    public:
        ~Options();
        Options();
        Options(int argc, char **argv);
        /**
         * @brief Returns true if launched with -h
         */
        bool PrintHelp();
        /**
         * @brief Returns name of file if used with -r or NULL
         * 
         * @return file name
         */
        char* GetFileName();
        /**
         * @brief Returns name of interface if used with -i or NULL
         * 
         * @return interface name
         */
        char* GetInterfaceName();
        /**
         * @brief Returns all prefixes
         * 
         * @return prefixes
         */
        std::set<char *> GetIPPrefixes();
};


#endif