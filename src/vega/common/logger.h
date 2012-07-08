#ifndef __VEGA_UTILS_LOGGER_H__
#define __VEGA_UTILS_LOGGER_H__

#include "types.h"
#include "singleton.h"

#define SMART_LOG_MSG(msg) vega::utils::smart_log fnlog(msg)
#define SMART_LOG_FN SMART_LOG_MSG(__FUNCTION__)

namespace vega
{
    namespace utils
    {
        class logger : public singleton<logger>
        {
            SINGLETON_DECL(logger);

            uint16 myIndentation;

        public:
            void initialize();
            void log(const char * msg);
            void caret_return() { myIndentation --; }
        };

        struct smart_log
        {
            smart_log(const char * msg) { logger::get()->log(msg); }
            ~smart_log() { logger::get()->caret_return(); }
        };
    }
}


#endif // __VEGA_UTILS_LOGGER_H__