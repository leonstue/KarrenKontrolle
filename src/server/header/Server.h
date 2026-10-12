#pragma once

#include <string>
#include <cstdint>

namespace karrenkontrolle::server
{
    class Server
    {
    public:
        void run(const std::string& ip, std::uint16_t port);
    };
}