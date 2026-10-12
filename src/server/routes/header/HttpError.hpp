#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace karrenkontrolle::server
{
    class HttpError : public std::runtime_error
    {
    public:
        HttpError(int status, std::string message) : std::runtime_error(std::move(message)), status_(status) {}

        int status() const noexcept
        {
            return status_;
        }

    private:
        int status_;
    };
}