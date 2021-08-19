//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <strf/to_string.hpp>
#include <strf/to_cfile.hpp>
#include <iostream>
#include <sstream>
#include <climits>
#include "fmt/format.h"
#include "fmt/compile.h"
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
                auto s = EXPR ;                                         \
                benchmark::DoNotOptimize(s.data());                     \
                benchmark::ClobberMemory();                             \
            }                                                           \
        }                                                               \
    };                                                                  \
    benchmark::RegisterBenchmark(LABEL, ID :: func);



#define FIXTURE_STR std::string str = "blah blah blah blah blah ";

#define FIXTURE_U8SAMPLE  auto u8sample = std::string(500, 'A');
#define FIXTURE_U16SAMPLE auto u16sample = std::string(500, 'A');

// static void u8_to_u16_buff(benchmark::State& state)
// {
//     char16_t u16buff [510];
//     FIXTURE_U8SAMPLE;
//     for(auto _ : state) {
//         (void)strf::to(u16buff)(strf::conv(u8sample));
//         auto str = strf::to_u16string.reserve_calc() (u16buff);
//         benchmark::DoNotOptimize(str.data());
//         benchmark::ClobberMemory();
//     }
// }

// static void u16_to_u8_buff(benchmark::State& state)
// {
//     char buff[510];
//     FIXTURE_U16SAMPLE;
//     for(auto _ : state) {
//         (void)strf::to(buff)(strf::conv(u16sample));
//         auto str = strf::to_string.reserve_calc()(buff);
//         benchmark::DoNotOptimize(str.data());
//         benchmark::ClobberMemory();
//     }
// }
constexpr double pi = M_PI;

int main(int argc, char** argv)
{
    BM(, std::to_string(123456));
    BM(, std::to_string(0.123456));

    BM(,strf::to_string(123456));
    BM(,strf::to_string(0.123456));
    BM(FIXTURE_STR, strf::to_string("Blah ", str, "!\n"));
    BM(FIXTURE_STR, strf::to_string("Blah ",  strf::right(str, 40, '.'), "!\n"));
    //BM(FIXTURE_STR, strf::to_string("Blah ",  strf::right(str, 40, U'\u2026'), "!\n"));
    BM(, strf::to_string("blah ", 123456, " blah ", 0x123456, " blah"));
    BM(, strf::to_string("blah ", +strf::dec(123456), " blah ", *strf::hex(0x123456), " blah"));
    BM(, strf::to_string("blah ", +strf::right(123456, 20, '_'), " blah ", *strf::hex(0x123456)<20, " blah"));
    BM(, strf::to_string(1.123e+5, ' ', pi, ' ', 1.11e-222));
    BM(, strf::to_string(*strf::fixed(1.123e+5), ' ', +strf::fixed(pi, 8), ' ', strf::sci(1.11e-222)>30));

    BM(, strf::to_string.tr("{}", 123456));
    BM(, strf::to_string.tr("{}", 0.123456));
    BM(FIXTURE_STR, strf::to_string.tr("Blah {}!\n", str));
    BM(FIXTURE_STR, strf::to_string.tr("Blah {}!\n", strf::right(str, 40, '.')));
    //BM(FIXTURE_STR, strf::to_string.tr("Blah {}!\n", strf::right(str, 40, U'\u2026') ));
    BM(, strf::to_string.tr("blah {} blah {} blah", 123456, 0x123456));
    BM(, strf::to_string.tr("blah {} blah {} blah", +strf::dec(123456), *strf::hex(0x123456)));
    BM(, strf::to_string.tr("blah {} blah {} blah", +strf::right(123456, 20, '_'), *strf::hex(0x123456)<20));
    BM(, strf::to_string.tr("{} {} {}", 1.123e+5, pi, 1.11e-222));
    BM(, strf::to_string.tr("{} {} {}", *strf::fixed(1.123e+5), +strf::fixed(pi, 8), strf::sci(1.11e-222)>30));

    BM2(,  fmt::format(FMT_COMPILE("{}"), 123456)
        , "fmt::format(FMT_COMPILE(\"{}\"), 123456)");
    BM2(,  fmt::format(FMT_COMPILE("{}"), 0.123456)
        , "fmt::format(FMT_COMPILE(\"{}\"), 0.123456)");
    BM2( FIXTURE_STR
       ,  fmt::format(FMT_COMPILE("Blah {}!\n"), str)
       , "fmt::format(FMT_COMPILE(\"Blah {}!\\n\"), str)");
    BM2(FIXTURE_STR
       ,  fmt::format(FMT_COMPILE("Blah {:.>40}!\n"), str)
       , "fmt::format(FMT_COMPILE(\"Blah {:.>40}!\\n\"), str)");
    // BM2(FIXTURE_STR
    //    ,  fmt::format(FMT_COMPILE("Blah {:\xE2\x80\xA6>40}!\n"), str)
    //    , "fmt::format(FMT_COMPILE(\"Blah {:\\xE2\\x80\\xA6>40}!\\n\"), str)");
    BM2(,  fmt::format(FMT_COMPILE("blah {} blah {} blah"), 123456, 0x123456)
        , "fmt::format(FMT_COMPILE(\"blah {} blah {} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format(FMT_COMPILE("blah {:+} blah {:#x} blah"), 123456, 0x123456)
        , "fmt::format(FMT_COMPILE(\"blah {:+} blah {:#x} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format(FMT_COMPILE("blah {:_>+20} blah {:<#20x} blah"), 123456, 0x123456)
        , "fmt::format(FMT_COMPILE(\"blah {:_>+20} blah {:<#20x} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format(FMT_COMPILE( "{} {} {}" ), 1.123e+5, pi, 1.11e-222)
        , "fmt::format(FMT_COMPILE(\"{} {} {}\"), 1.123e+5, pi, 1.11e-222)");
    BM2(,  fmt::format(FMT_COMPILE( "{:#f} {:+.8f} {:>30e}" ), 1.123e+5, pi, 1.11e-222)
        , "fmt::format(FMT_COMPILE(\"{:#f} {:+.8f} {:>30e}\"), 1.123e+5, pi, 1.11e-222)");

    BM(, fmt::format("{}", 123456));
    BM(, fmt::format("{}", 0.123456));
    BM(FIXTURE_STR, fmt::format("Blah {}!\n", str));
    BM(FIXTURE_STR, fmt::format("Blah {:.>40}!\n", str));
    // BM(FIXTURE_STR, fmt::format("Blah {:\xE2\x80\xA6>40}!\n", str));
    BM(, fmt::format("blah {} blah {} blah", 123456, 0x123456));
    BM(, fmt::format("blah {:+} blah {:#x} blah", 123456, 0x123456));
    BM(, fmt::format("blah {:_>+20} blah {:<#20x} blah", 123456, 0x123456));
    BM(, fmt::format("{} {} {}", 1.123e+5, pi, 1.11e-222));
    BM(, fmt::format("{:#f} {:+.8f} {:>30e}", 1.123e+5, pi, 1.11e-222));

    BM(FIXTURE_U8SAMPLE, strf::to_u16string.reserve_calc() (strf::conv(u8sample)));
    BM(FIXTURE_U8SAMPLE, strf::to_u16string.no_reserve()   (strf::conv(u8sample)));
    BM(FIXTURE_U8SAMPLE, strf::to_u16string.reserve(510)   (strf::conv(u8sample)));

    BM(FIXTURE_U16SAMPLE, strf::to_string.reserve_calc() (strf::conv(u16sample)));
    BM(FIXTURE_U16SAMPLE, strf::to_string.no_reserve()   (strf::conv(u16sample)));
    BM(FIXTURE_U16SAMPLE, strf::to_string.reserve(510)   (strf::conv(u16sample)));

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    strf::to(stdout)
        ( "\n  where :"
          "\n    constexpr double pi = M_PI;"
          "\n    " STR(FIXTURE_STR)
          "\n    " STR(FIXTURE_U8SAMPLE)
          "\n    " STR(FIXTURE_U16SAMPLE)
          "\n" );

    return 0;
}

