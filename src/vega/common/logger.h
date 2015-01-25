#ifndef __VEGA_UTILS_LOGGER_H__
#define __VEGA_UTILS_LOGGER_H__

#include "types.h"
#include "singleton.h"

#define VEGA_LOG(ch, msg) vega::utils::logger::get()->log(vega::utils::ch, (msg))

#define VEGA_LOG_VERBOSE(msg) VEGA_LOG(LOG_VERBOSE, msg)
#define VEGA_LOG_DEBUG(msg) VEGA_LOG(LOG_DEBUG, msg)
#define VEGA_LOG_INFO(msg) VEGA_LOG(LOG_INFO, msg)
#define VEGA_LOG_WARN(msg) VEGA_LOG(LOG_WARN, msg)
#define VEGA_LOG_ERROR(msg) VEGA_LOG(LOG_ERROR, msg)

#define VEGA_LOG_FN VEGA_LOG_VERBOSE(__FUNCTION__)


namespace vega
{
    namespace utils
    {
        enum LOG_CHANNEL
        {
            LOG_VERBOSE,
            LOG_DEBUG,
            LOG_INFO,
            LOG_WARN,
            LOG_ERROR,
            LOG_ASSERT,
        };

        class logger : public singleton<logger>
        {
            SINGLETON_DECL(logger);

            HANDLE hConsole;
        public:
            ~logger();

            void initialize();
            void log(LOG_CHANNEL channel, const char * msg);
        };
    }
}


#endif // __VEGA_UTILS_LOGGER_H__