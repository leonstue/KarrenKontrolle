#pragma once

namespace karrenkontrolle::server
{
    template<typename T> struct HttpResult
    {
        int status = 200;
        T body;
    };
}