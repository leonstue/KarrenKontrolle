#pragma once

#include <string>
#include <cstdint>

namespace karrenkontrolle::config
{
    struct AppConfig
    {
        std::string ip = "0.0.0.0";
        std::uint16_t port = 8080;
    };

    AppConfig parseArguments(int argc, char* argv[]);
}