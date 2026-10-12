#pragma once

#include <rfl/json.hpp>
#include <string>

template <typename T>
struct JsonSerializer
{
    static std::string serialize(const T &data)
    {
        return rfl::json::write(data);
    }
};