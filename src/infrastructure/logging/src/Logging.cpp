#include "Logging.h"

#include <trantor/utils/Logger.h>

namespace karrenkontrolle::logging
{
    void initialize(){
        trantor::Logger::setLogLevel(trantor::Logger::kDebug);
        trantor::Logger::setDisplayLocalTime(true);

        LOG_INFO << "Logging initalized";
    }
}