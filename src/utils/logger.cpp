#include <iostream>
#include "logger.h"

void Logger::log(std::string s)
{
    std::cout << s << std::endl;
}

void Logger::log(int s)
{
    std::cout << s << std::endl;
}