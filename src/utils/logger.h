#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
    public:
    Logger() = delete;
    static void log(std::string);
    static void log(int);
};

#endif //LOGGER_H