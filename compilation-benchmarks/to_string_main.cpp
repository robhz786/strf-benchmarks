// Distributed under the Boost Software License, Version 1.0.
// ( See accompanying file LICENSE or copy at
//   http://www.boost.org/LICENSE_1_0.txt )

#include <cstdio>
#include <string>

using output_type = std::string&;

#include "tmp/functions_declations.hpp"

const char* format_string()
{
    return "";
}

int main()
{
    std::string destination;
    destination.reserve(10000);

#include "tmp/functions_calls.cpp"

    puts(destination.c_str());
    return 0;
}


