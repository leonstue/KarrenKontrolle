#include "Server.h"
#include "Logging.h"

int main()
{

    karrenkontrolle::logging::initialize();

    karrenkontrolle::server::Server server;
    server.run();

    return 0;
}