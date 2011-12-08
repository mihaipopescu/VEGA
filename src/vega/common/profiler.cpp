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
        uint64 dt = qwTime.QuadPart - it->second.start;
        if( dt > it->second.elapsed )
            it->second.elapsed = dt;

        if( myDumpOnEnd )
        {
            // after dumping the report, we clear the elapsed
            dump_key(key);
            it->second.elapsed = 0;
        }
    }
}

void vega::utils::profiler::_profile_entry::dump( const profile_map::value_type& p )
{
    LARGE_INTEGER qwTicksPerSec = { 0 };
    QueryPerformanceFrequency( &qwTicksPerSec );
    double fFrequency = (double) qwTicksPerSec.QuadPart;

    float fElapsedTime = ( float )( ( double )( p.second.elapsed ) / fFrequency);

    if( fElapsedTime > 0.0f )
    {
        std::cout << "Profile key[" << p.first << "] = " << std::setprecision(2) << fElapsedTime << "s" << std::endl;
    }
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
