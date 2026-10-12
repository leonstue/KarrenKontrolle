
#include "AppConfig.h"
#include "Server.h"
#include "Logging.h"

#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        auto config = karrenkontrolle::config::parseArguments(argc, argv);

        karrenkontrolle::logging::initialize();

        karrenkontrolle::server::Server server;
        server.run(config.ip, config.port);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Startup failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
