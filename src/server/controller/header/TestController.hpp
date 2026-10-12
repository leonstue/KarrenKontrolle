#pragma once

#include "MessageResponse.hpp"
#include "HttpError.hpp"
#include <string>

namespace karrenkontrolle::server::controller
{
    inline MessageResponse ping()
    {
        return {"pong"};
    }

    inline MessageResponse echo(const std::string& message)
    {
        if (message.empty()){
            throw HttpError(400, "Echo message ist empty. Don't!");
        }

        return {message};
    }

}