#include "Server.h"
#include "Routes.h"

#include <drogon/drogon.h>

namespace karrenkontrolle::server
{
    void Server::run()
    {
        constexpr auto host = "127.0.0.1";
        constexpr auto port = 8080;

        registerRoutesV1();

        LOG_INFO << "Starting server";
        LOG_INFO << "Listening on " << host << ":" << port;

        drogon::app()
            .addListener(host, port)
            .run();
    }
}