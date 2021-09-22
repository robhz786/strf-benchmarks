//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <strf/to_cfile.hpp>
#include <benchmark/benchmark.h>

#define STR2(X) #X
#define STR(X) STR2(X)
#define CAT2(X, Y) X##Y
#define CAT(X,Y) CAT2(X,Y)
#define BM(FIXTURE, EXPR)  BM2(FIXTURE, EXPR, STR(EXPR))
#define BM2(FIXTURE, EXPR, LABEL)  DO_BM(CAT(bm_, __LINE__), FIXTURE, EXPR, LABEL)
#define DO_BM(ID, FIXTURE, EXPR, LABEL)                                   \
    struct ID {                                                         \
        static void func(benchmark::State& state) {                     \
            FIXTURE;                                                    \
            for(auto _ : state) {                                       \
                EXPR ;                                                  \
                benchmark::DoNotOptimize(dest);                         \
                benchmark::ClobberMemory();                             \
            }                                                           \
        }                                                               \
    };                                                                  \
    benchmark::RegisterBenchmark(LABEL, ID :: func);


constexpr auto fast_width  = strf::fast_width;
constexpr auto slow_u32len = strf::width_as_fast_u32len;
constexpr auto fast_u32len = strf::width_as_u32len;
auto wfunc = [](char32_t ch) { return strf::width_t(ch == U'\u2E3A' ? 4 : ch == U'\u2014' ? 2 : 1); };
const auto custom_calc = strf::make_width_calculator(wfunc);
constexpr auto std_width_calc = strf::std_width_calc;

const std::string u8str5  (5, 'x');
const std::string u8str50 (50, 'x');
const std::u16string u16str5  (5, u'x');
const std::u16string u16str50 (50, u'x');

int main(int argc, char** argv)
{
    BM(char dest[100];, strf::to(dest) .with( fast_width)    (strf::fmt(u8str5) > 5));
    BM(char dest[100];, strf::to(dest) .with(fast_u32len)    (strf::fmt(u8str5) > 5));
    BM(char dest[100];, strf::to(dest) .with(slow_u32len)    (strf::fmt(u8str5) > 5));
    BM(char dest[100];, strf::to(dest) .with(custom_calc)    (strf::fmt(u8str5) > 5));
    BM(char dest[100];, strf::to(dest) .with(std_width_calc) (strf::fmt(u8str5) > 5));

    BM(char dest[100];, strf::to(dest) .with( fast_width)    (strf::join_right(5)(u8str5)));
    BM(char dest[100];, strf::to(dest) .with(fast_u32len)    (strf::join_right(5)(u8str5)));
    BM(char dest[100];, strf::to(dest) .with(slow_u32len)    (strf::join_right(5)(u8str5)));
    BM(char dest[100];, strf::to(dest) .with(custom_calc)    (strf::join_right(5)(u8str5)));
    BM(char dest[100];, strf::to(dest) .with(std_width_calc) (strf::join_right(5)(u8str5)));

    BM(char dest[100];, strf::to(dest) .with( fast_width)    (strf::fmt(u8str50) > 50));
    BM(char dest[100];, strf::to(dest) .with(fast_u32len)    (strf::fmt(u8str50) > 50));
    BM(char dest[100];, strf::to(dest) .with(slow_u32len)    (strf::fmt(u8str50) > 50));
    BM(char dest[100];, strf::to(dest) .with(custom_calc)    (strf::fmt(u8str50) > 50));
    BM(char dest[100];, strf::to(dest) .with(std_width_calc) (strf::fmt(u8str50) > 50));

    BM(char dest[100];, strf::to(dest) .with( fast_width)    (strf::join_right(50)(u8str50)));
    BM(char dest[100];, strf::to(dest) .with(fast_u32len)    (strf::join_right(50)(u8str50)));
    BM(char dest[100];, strf::to(dest) .with(slow_u32len)    (strf::join_right(50)(u8str50)));
    BM(char dest[100];, strf::to(dest) .with(custom_calc)    (strf::join_right(50)(u8str50)));
    BM(char dest[100];, strf::to(dest) .with(std_width_calc) (strf::join_right(50)(u8str50)));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::fmt(u16str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::fmt(u16str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::fmt(u16str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::fmt(u16str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::fmt(u16str5) > 5));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::join_right(5)(u16str5)));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::join_right(5)(u16str5)));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::join_right(5)(u16str5)));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::join_right(5)(u16str5)));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::join_right(5)(u16str5)));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::fmt(u16str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::fmt(u16str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::fmt(u16str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::fmt(u16str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::fmt(u16str50) > 50));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::join_right(50)(u16str50)));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::join_right(50)(u16str50)));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::join_right(50)(u16str50)));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::join_right(50)(u16str50)));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::join_right(50)(u16str50)));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::conv(u8str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::conv(u8str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::conv(u8str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::conv(u8str5) > 5));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::conv(u8str5) > 5));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::join_right(5)(strf::conv(u8str5))));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::join_right(5)(strf::conv(u8str5))));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::join_right(5)(strf::conv(u8str5))));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::join_right(5)(strf::conv(u8str5))));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::join_right(5)(strf::conv(u8str5))));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::conv(u8str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::conv(u8str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::conv(u8str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::conv(u8str50) > 50));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::conv(u8str50) > 50));

    BM(char16_t dest[100];, strf::to(dest).with( fast_width)    (strf::join_right(50)(strf::conv(u8str50))));
    BM(char16_t dest[100];, strf::to(dest).with(fast_u32len)    (strf::join_right(50)(strf::conv(u8str50))));
    BM(char16_t dest[100];, strf::to(dest).with(slow_u32len)    (strf::join_right(50)(strf::conv(u8str50))));
    BM(char16_t dest[100];, strf::to(dest).with(custom_calc)    (strf::join_right(50)(strf::conv(u8str50))));
    BM(char16_t dest[100];, strf::to(dest).with(std_width_calc) (strf::join_right(50)(strf::conv(u8str50))));

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}
