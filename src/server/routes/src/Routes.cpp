#include "Routes.h"
#include "Router.hpp"
#include "TestController.hpp"
#include <string>

namespace karrenkontrolle::server
{

    void registerRoutesV1()
    {
        const std::string apiPrefix = "api/v1";
        Router router(apiPrefix);

        // Simple get test controller
        router.get("/api/v1/ping", controller::ping);

        // Get testcontroller with a path variable
        router.get<std::string>(
            "/echo/{message}",
            controller::echo);

        // Get testcontroller with error return
        router.get("/echo", []()
                   { return controller::echo(""); });
    }
    
}