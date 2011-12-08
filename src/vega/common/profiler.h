#ifndef __VEGA_UTILS_PROFILER_H__
#define __VEGA_UTILS_PROFILER_H__

#include "types.h"
#include "singleton.h"
#include <map>


#define PROFILE_BEGIN(key) vega::utils::profiler::get()->begin_profile(key);
#define PROFILE_END(key) vega::utils::profiler::get()->end_profile(key);
#define PROFILE_SMART(key) vega::utils::profile_smart profile_smartobj_##key(key);

namespace vega
{
    namespace utils
    {
        /// This is the profiler singleton class
        class profiler : public singleton<profiler>
        {
            SINGLETON_DECL(profiler);

            // the profiler map keeps a mapping between the profile ID and the maximum time hit
            struct _profile_entry;
            typedef std::map<uint32, _profile_entry> profile_map;

            struct _profile_entry
            {
                _profile_entry(uint64 _now) : start(_now), elapsed(0ul) { }
                
                uint64 start;
                uint64 elapsed;

                static void dump(const profile_map::value_type& p);
            };

        public:
            void set_dump_on_end(bool bValue) { myDumpOnEnd = bValue; }
            void begin_profile(uint32 key);
            void end_profile(uint32 key);
            void dump_map() const;
            void dump_key(uint32 key) const;

        protected:
            profile_map myProfileMap;
            bool myDumpOnEnd;
        };

        class profile_smart
        {
        public:
            profile_smart(uint32 key) : myKey(key) { profiler::get()->begin_profile(key); }
            ~profile_smart() { profiler::get()->end_profile(myKey); }
        protected:
            uint32 myKey;
        };
    }
}

#endif