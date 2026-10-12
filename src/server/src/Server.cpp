#include "Server.h"
#include "Routes.h"
#include "AuthGuard.h"

#include <drogon/drogon.h>

namespace karrenkontrolle::server
{
    void Server::run(const std::string& ip, std::uint16_t port)
    {

        //register middleware
        middleware::registerAuthGuard();

        //register routes
        registerRoutesV1();

        LOG_INFO << "Starting server";
        LOG_INFO << "Listening on " << ip << ":" << port;

        drogon::app()
            .addListener(ip, port)
            .run();
    }
}