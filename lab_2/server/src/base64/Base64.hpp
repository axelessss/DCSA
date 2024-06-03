#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace base64
{
    std::string Encode(const std::string &txt);

    std::string Decode(const std::string &txt);

    std::vector<std::string> Split(const std::string &s, const char *delim);
}