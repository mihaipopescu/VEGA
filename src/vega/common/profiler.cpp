#include "profiler.h"
#include <windows.h>
#include <winbase.h>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

SINGLETON_IMPL(vega::utils::profiler)

using namespace vega;
using namespace vega::utils;

void profiler::begin_profile(uint32 key)
{
    // Get the current time
    LARGE_INTEGER qwTime = { 0 };
    QueryPerformanceCounter( &qwTime );

    profile_map::iterator it = myProfileMap.find(key);

    if( it == myProfileMap.end() )
    {
        myProfileMap.insert(std::pair<profile_map::key_type, profile_map::mapped_type>(key, _profile_entry(qwTime.QuadPart)));
    }
    else
    {
        // update the profile entry leaving the elapsed unmodified
        it->second.start = qwTime.QuadPart;
        it->second.count++;
    }
}

void profiler::end_profile(uint32 key)
{
    // Get the current time
    LARGE_INTEGER qwTime = { 0 };
    QueryPerformanceCounter( &qwTime );

    profile_map::iterator it = myProfileMap.find(key);
    if( it == myProfileMap.end() )
    {
        assert(false);
    }
    else
    {
        _profile_entry & entry = it->second;

        uint64 dt = qwTime.QuadPart - entry.start;
        entry.elapsed = dt;

        if( dt > entry.peak )
            entry.peak = dt;

        if( myDumpOnEnd )
        {
            // after dumping the report, we clear the elapsed peak
            dump_key(key);
            it->second.peak = 0;
        }
    }
}

void vega::utils::profiler::_profile_entry::dump( const profile_map::value_type& p )
{
    float fElapsedTime = p.second.get_elapsed_time();
    if( fElapsedTime > 0.0f )
    {
        std::cout << "Profile key[" << p.first << "] = " << std::setprecision(2) << fElapsedTime << "s" << std::endl;
    }
}

float vega::utils::profiler::_profile_entry::get_elapsed_time() const
{
    LARGE_INTEGER qwTicksPerSec = { 0 };
    QueryPerformanceFrequency( &qwTicksPerSec );
    double fFrequency = (double) qwTicksPerSec.QuadPart;

    if ( fFrequency > 0)
        return ( float )( ( double )( elapsed ) / fFrequency);
    else
        return -1.f;
}

void profiler::dump_map() const
{
    std::for_each(myProfileMap.begin(), myProfileMap.end(), _profile_entry::dump);
}

void vega::utils::profiler::dump_key( uint32 key ) const
{
    auto it = myProfileMap.find(key);
    if( it != myProfileMap.end() )
    {
        _profile_entry::dump(*it);
    }
}

float vega::utils::profiler::get_elapsed_time_by_key( uint32 key ) const
{
    auto it = myProfileMap.find(key);
    if( it != myProfileMap.end() )
    {
        return it->second.get_elapsed_time();
    }

    return -1.f;
}
