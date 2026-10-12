
#include "AppConfig.h"

#include <charconv>
#include <stdexcept>
#include <string_view>
#include <system_error>

namespace karrenkontrolle::config
{
    AppConfig parseArguments(int argc, char* argv[])
    {
        AppConfig config;

        for (int i = 1; i < argc; ++i)
        {
            std::string_view arg = argv[i];

            if (arg == "--ip")
            {
                if (i + 1 >= argc ||
                    std::string_view(argv[i + 1]).starts_with("--"))
                {
                    throw std::invalid_argument("Missing value for --ip");
                }

                config.ip = argv[++i];
            }
            else if (arg == "--port")
            {
                if (i + 1 >= argc)
                {
                    throw std::invalid_argument("Missing value for --port");
                }

                std::string_view value = argv[++i];
                unsigned int port = 0;

                auto [ptr, ec] = std::from_chars(
                    value.data(),
                    value.data() + value.size(),
                    port
                );

                if (ec != std::errc{} ||
                    ptr != value.data() + value.size() ||
                    port == 0 || port > 65535)
                {
                    throw std::invalid_argument("Invalid port: " + std::string(value));
                }

                config.port = static_cast<std::uint16_t>(port);
            }
            else
            {
                throw std::invalid_argument("Unknown argument: " + std::string(arg));
            }
        }

        return config;
    }
}
