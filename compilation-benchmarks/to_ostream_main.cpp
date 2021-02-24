// Distributed under the Boost Software License, Version 1.0.
// ( See accompanying file LICENSE or copy at
//   http://www.boost.org/LICENSE_1_0.txt )

#include <iostream>

using output_type = std::ostream&;

#include "tmp/functions_declations.hpp"

const char* format_string()
{
    return "";
}

int main()
{
    std::ostream & destination = std::cout;

#include "tmp/functions_calls.cpp"

    return 0;
}


