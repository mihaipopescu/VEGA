#include "logger.h"
#include <iostream>

SINGLETON_IMPL(vega::utils::logger);


void vega::utils::logger::initialize()
{
    myIndentation = 0;
}

void vega::utils::logger::log( const char * text )
{
    for(int i=0;i<myIndentation; i++)
    {
        std::cout << "\t";
    }
    
    std::cout << text << std::endl;
    myIndentation ++;
}
