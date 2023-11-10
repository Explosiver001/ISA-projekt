/**
 * @file logger.h
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Declaration of logging tool
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _EVENT_LOGGER_H_
#define _EVENT_LOGGER_H_
#include <vector>
#include <cstdint>

/**
 * @brief Log information to console and syslog
 * 
 */
class EventLogger{
    private:
        int _prefix_lines; //store count of prefixes

    public:
        /**
         * @brief Construct a new Event Logger:: Event Logger object
         * 
         */
        EventLogger();

        /**
         * @brief Print notice to syslog
         * 
         * @param base 
         * @param mask_size 
         */
        void Log50Exceeded(char * prefix);

        /**
         * @brief Write 
         * 
         * @param prefixes 
         * @param max_ips 
         */
        void InitConsoleOutput(std::vector<char *> prefixes, std::vector<uint32_t> max_ips);

        /**
         * @brief Updates output on specified line
         * 
         * @param prefix_num order of the prefix
         * @param prefix 
         * @param max_devices 
         * @param devices 
         */
        void UpdateLine(int prefix_num, char* prefix, uint32_t max_devices, uint32_t devices);

        /**
         * @brief Used when reading from files to freeze output
         * 
         */
        void NotifyFileEnd();

        ~EventLogger();
};

#endif