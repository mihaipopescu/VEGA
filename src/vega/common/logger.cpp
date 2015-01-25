#include "logger.h"
#include "profiler.h"

#include <windows.h>
#include <iostream>
#include <iomanip>



SINGLETON_IMPL(vega::utils::logger);

#define LOGGER_PROFILE_KEY 0x105

void vega::utils::logger::initialize()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    profiler::get()->begin_profile(LOGGER_PROFILE_KEY);
}

void vega::utils::logger::log(LOG_CHANNEL channel, const char * msg)
{
    switch(channel)
    {
    case LOG_INFO:
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
        break;
    case LOG_DEBUG:
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        break;
    case LOG_WARN:
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case LOG_ERROR:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    default:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    }

    profiler::get()->end_profile(LOGGER_PROFILE_KEY);
    uint32 ums = (uint32)(profiler::get()->get_elapsed_time_by_key(LOGGER_PROFILE_KEY) * 1000);
    std::cout.width(6);
    std::cout << ums << "ms: " << msg << std::endl;
    profiler::get()->begin_profile(LOGGER_PROFILE_KEY);
}

vega::utils::logger::~logger()
{
    if( profiler::get() ) 
    {
        profiler::get()->end_profile(LOGGER_PROFILE_KEY);
        std::cout << std::setprecision(2) << profiler::get()->get_elapsed_time_by_key(LOGGER_PROFILE_KEY) << ": Program terminated !" << std::endl;
    }
}
