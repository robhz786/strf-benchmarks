// Distributed under the Boost Software License, Version 1.0.
// ( See accompanying file LICENSE or copy at
//   http://www.boost.org/LICENSE_1_0.txt )

#include <cstdio>

using output_type = int; // dummy type

#include "tmp/functions_declations.hpp"

const char* format_string()
{
    return "";
}

int main()
{
    int destination = 0;

#include "tmp/functions_calls.cpp"

    return 0;
}


