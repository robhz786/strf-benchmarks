//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <strf/to_cfile.hpp>
#include <strf/iterator.hpp>
#include <stdio.h>
#include <climits>
#include "fmt/format.h"
#include "fmt/compile.h"
#include <benchmark/benchmark.h>

constexpr std::size_t dest_size = 110;

#define STR2(X) #X
#define STR(X) STR2(X)
#define CAT2(X, Y) X##Y
#define CAT(X,Y) CAT2(X,Y)
#define BM(FIXTURE, EXPR)  BM2(FIXTURE, EXPR, STR(EXPR))
#define BM2(FIXTURE, EXPR, LABEL)  DO_BM(CAT(bm_, __LINE__), FIXTURE, EXPR, LABEL)
#define DO_BM(ID, FIXTURE, EXPR, LABEL)                                 \
    struct ID {                                                         \
        static void func(benchmark::State& state) {                     \
            FIXTURE;                                                    \
            char dest[dest_size];                                       \
            for(auto _ : state) {                                       \
                EXPR ;                                                  \
                benchmark::DoNotOptimize(dest);                         \
                benchmark::ClobberMemory();                             \
            }                                                           \
        }                                                               \
    };                                                                  \
    benchmark::RegisterBenchmark(LABEL, ID :: func);


#define BM_HYBRID(FIXTURE, ...)             \
    DO_BM_HYBRID(CAT(bm_, __LINE__), FIXTURE, __VA_ARGS__)

#define DO_BM_HYBRID(ID, FIXTURE, ...)                                  \
    struct ID {                                                         \
        static void func(benchmark::State& state) {                     \
            FIXTURE;                                                    \
            char dest[dest_size];                                       \
            for(auto _ : state) {                                       \
                strf::cstr_writer dst{dest, dest_size};                 \
                fmt::format_to(strf::make_iterator(dst), __VA_ARGS__);  \
                auto res = dst.finish();                                \
                benchmark::DoNotOptimize(&res);                         \
                benchmark::ClobberMemory();                             \
            }                                                           \
        }                                                               \
    };                                                                  \
    benchmark::RegisterBenchmark("hybrid_fmt_strf(" #__VA_ARGS__ ")", ID :: func);

#define BM_HYBRID_COMPILE(FIXTURE, FMT, ...)                 \
    DO_BM_HYBRID_COMPILE(CAT(bm_, __LINE__), FIXTURE, FMT, __VA_ARGS__)

#define DO_BM_HYBRID_COMPILE(ID, FIXTURE, FMT, ...)                     \
    struct ID {                                                         \
        static void func(benchmark::State& state) {                     \
            FIXTURE;                                                    \
            char dest[dest_size];                                       \
            for(auto _ : state) {                                       \
                strf::cstr_writer dst{dest, dest_size};                 \
                fmt::format_to( strf::make_iterator(dst)                \
                              , FMT_COMPILE(FMT), __VA_ARGS__);         \
                auto res = dst.finish();                                \
                benchmark::DoNotOptimize(&res);                         \
                benchmark::ClobberMemory();                             \
            }                                                           \
        }                                                               \
    };                                                                  \
    benchmark::RegisterBenchmark                                        \
        ( "hybrid_fmt_strf(COMPILE(" #FMT "), " #__VA_ARGS__ ")"        \
        , ID :: func );


#define FIXTURE_STR std::string str = "blah blah blah blah blah ";
constexpr double pi = M_PI;

int main(int argc, char** argv)
{
    BM(FIXTURE_STR, strf::to(dest, dest_size)("Blah ", str, "!\n"));
    BM(FIXTURE_STR, strf::to(dest, dest_size)("Blah ", strf::right(str, 40, '.'), "!\n"));
    // BM(FIXTURE_STR, strf::to(dest, dest_size)("Blah ", strf::right(str, 40, U'\u2026'), "!\n"));
    BM(, strf::to(dest, dest_size)("blah ", 123456, " blah ", 0x123456, " blah"));
    BM(, strf::to(dest, dest_size)("blah ", +strf::dec(123456), " blah ", *strf::hex(0x123456), " blah"));
    BM(, strf::to(dest, dest_size)("blah ", +strf::right(123456, 20, '_'), " blah ", *strf::hex(0x123456)<20, " blah"));
    BM(, strf::to(dest, dest_size)(1.123e+5, ' ', pi, ' ', 1.11e-222));
    BM(, strf::to(dest, dest_size)(*strf::fixed(1.123e+5), ' ', +strf::fixed(pi, 8), ' ', strf::sci(1.11e-222)>30));

    BM(FIXTURE_STR, strf::to(dest, dest_size).tr("Blah {}!\n", str));
    BM(FIXTURE_STR, strf::to(dest, dest_size).tr("Blah {}!\n", strf::right(str, 40, '.')));
    // BM(FIXTURE_STR, strf::to(dest, dest_size).tr("Blah {}!\n", strf::right(str, 40, U'\u2026')));
    BM(, strf::to(dest, dest_size).tr("blah {} blah {} blah", 123456, 0x123456));
    BM(, strf::to(dest, dest_size).tr("blah {} blah {} blah", +strf::dec(123456), *strf::hex(0x123456)));
    BM(, strf::to(dest, dest_size).tr("blah {} blah {} blah", +strf::right(123456, 20, '_'), *strf::hex(0x123456)<20));
    BM(, strf::to(dest, dest_size).tr("{} {} {}", 1.123e+5, pi, 1.11e-222));
    BM(, strf::to(dest, dest_size).tr("{} {} {}", *strf::fixed(1.123e+5), +strf::fixed(pi, 8), strf::sci(1.11e-222)>30));

    BM2(FIXTURE_STR, fmt::format_to(dest, FMT_COMPILE( "Blah {}!\n" ), str)
                  , "fmt::format_to(dest, FMT_COMPILE(\"Blah {}!\\n\"), str)");
    BM2(FIXTURE_STR, fmt::format_to(dest, FMT_COMPILE( "Blah {:.>40}!\n" ), str)
                  , "fmt::format_to(dest, FMT_COMPILE(\"Blah {:.>40}!\\n\"), str)");
    // BM2(FIXTURE_STR,  fmt::format_to(dest, FMT_COMPILE( "Blah {:\xE2\x80\xA6>40}!\n" ), str)
    //                , "fmt::format_to(dest, FMT_COMPILE(\"Blah {:\\xE2\\x80\\xA6>40}!\\n\"), str)");
    BM2(,  fmt::format_to(dest, FMT_COMPILE( "blah {} blah {} blah" ), 123456, 0x123456)
        , "fmt::format_to(dest, FMT_COMPILE(\"blah {} blah {} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format_to(dest, FMT_COMPILE( "blah {:+} blah {:#x} blah" ), 123456, 0x123456)
        , "fmt::format_to(dest, FMT_COMPILE(\"blah {:+} blah {:#x} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format_to(dest, FMT_COMPILE( "blah {:_>+20} blah {:<#20x} blah" ), 123456, 0x123456)
        , "fmt::format_to(dest, FMT_COMPILE(\"blah {:_>+20} blah {:<#20x} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format_to(dest, FMT_COMPILE( "{} {} {}" ), 1.123e+5, pi, 1.11e-222)
        , "fmt::format_to(dest, FMT_COMPILE(\"{} {} {}\"), 1.123e+5, pi, 1.11e-222)");
    BM2(,  fmt::format_to(dest, FMT_COMPILE( "{:#f} {:+.8f} {:>30e}" ), 1.123e+5, pi, 1.11e-222)
        , "fmt::format_to(dest, FMT_COMPILE(\"{:#f} {:+.8f} {:>30e}\"), 1.123e+5, pi, 1.11e-222)");

    BM(FIXTURE_STR, fmt::format_to(dest, "Blah {}!\n", str));
    BM(FIXTURE_STR, fmt::format_to(dest, "Blah {:.>40}!\n", str));
    // BM(FIXTURE_STR, fmt::format_to(dest, "Blah {:\xE2\x80\xA6>40}!\n", str));
    BM(, fmt::format_to(dest, "blah {} blah {} blah", 123456, 0x123456));
    BM(, fmt::format_to(dest, "blah {:+} blah {:#x} blah", 123456, 0x123456));
    BM(, fmt::format_to(dest, "blah {:_>+20} blah {:<#20x} blah", 123456, 0x123456));
    BM(, fmt::format_to(dest,  "{} {} {}", 1.123e+5, pi, 1.11e-222));
    BM(, fmt::format_to(dest,  "{:#f} {:+.8f} {:>30e}", 1.123e+5, pi, 1.11e-222));

    BM2( FIXTURE_STR
       ,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "Blah {}!\n" ), str)
       , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"Blah {}!\\n\"), str)");
    BM2(FIXTURE_STR
       ,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "Blah {:.>40}!\n" ), str)
       , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"Blah {:.>40}!\\n\"), str)");
    // BM2(FIXTURE_STR
    //    ,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "Blah {:\xE2\x80\xA6>40}!\n" ), str)
    //    , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"Blah {:\\xE2\\x80\\xA6>40}!\\n\"), str)");
    BM2(,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "blah {} blah {} blah" ), 123456, 0x123456)
        , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"blah {} blah {} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "blah {:+} blah {:#x} blah" ), 123456, 0x123456)
        , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"blah {:+} blah {:#x} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "blah {:_>+20} blah {:<#20x} blah" ), 123456, 0x123456)
        , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"blah {:_>+20} blah {:<#20x} blah\"), 123456, 0x123456)");
    BM2(,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "{} {} {}" ), 1.123e+5, pi, 1.11e-222)
        , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"{} {} {}\"), 1.123e+5, pi, 1.11e-222)");
    BM2(,  fmt::format_to_n(dest, dest_size, FMT_COMPILE( "{:#f} {:+.8f} {:>30e}" ), 1.123e+5, pi, 1.11e-222)
        , "fmt::format_to_n(dest, dest_size, FMT_COMPILE(\"{:#f} {:+.8f} {:>30e}\"), 1.123e+5, pi, 1.11e-222)");

    BM(FIXTURE_STR, fmt::format_to_n(dest, dest_size, "Blah {}!\n", str));
    BM(FIXTURE_STR, fmt::format_to_n(dest, dest_size, "Blah {:.>40}!\n", str));
    // BM(FIXTURE_STR, fmt::format_to_n(dest, dest_size, "Blah {:\xE2\x80\xA6>40}!\n", str));
    BM(, fmt::format_to_n(dest, dest_size, "blah {} blah {} blah", 123456, 0x123456));
    BM(, fmt::format_to_n(dest, dest_size, "blah {:+} blah {:#x} blah", 123456, 0x123456));
    BM(, fmt::format_to_n(dest, dest_size, "blah {:_>+20} blah {:<#20x} blah", 123456, 0x123456));
    BM(, fmt::format_to_n(dest, dest_size, "{} {} {}", 1.123e+5, pi, 1.11e-222));
    BM(, fmt::format_to_n(dest, dest_size, "{:#f} {:+.8f} {:>30e}", 1.123e+5, pi, 1.11e-222));

    BM_HYBRID_COMPILE(FIXTURE_STR, "Blah {}!\n", str);
    BM_HYBRID_COMPILE(FIXTURE_STR, "Blah {:.>40}!\n", str);
    //BM_HYBRID_COMPILE(FIXTURE_STR, "Blah {:\xE2\x80\xA6>40}!\n", str);
    BM_HYBRID_COMPILE(, "blah {} blah {} blah", 123456, 0x123456);
    BM_HYBRID_COMPILE(, "blah {:+} blah {:#x} blah", 123456, 0x123456);
    BM_HYBRID_COMPILE(, "blah {:_>+20} blah {:<#20x} blah", 123456, 0x123456);
    BM_HYBRID_COMPILE(, "{} {} {}", 1.123e+5, pi, 1.11e-222);
    BM_HYBRID_COMPILE(, "{:#f} {:+.8f} {:>30e}", 1.123e+5, pi, 1.11e-222);

    BM_HYBRID(FIXTURE_STR, "Blah {}!\n", str);
    BM_HYBRID(FIXTURE_STR, "Blah {:.>40}!\n", str);
    //_HYBRID BM(FIXTURE_STR, "Blah {:\xE2\x80\xA6>40}!\n", str);
    BM_HYBRID(, "blah {} blah {} blah", 123456, 0x123456);
    BM_HYBRID(, "blah {:+} blah {:#x} blah", 123456, 0x123456);
    BM_HYBRID(, "blah {:_>+20} blah {:<#20x} blah", 123456, 0x123456);
    BM_HYBRID(, "{} {} {}", 1.123e+5, pi, 1.11e-222);
    BM_HYBRID(, "{:#f} {:+.8f} {:>30e}", 1.123e+5, pi, 1.11e-222);

    BM(FIXTURE_STR, std::sprintf(dest, "Blah %s!\n", str.c_str()));
    BM(FIXTURE_STR, std::sprintf(dest, "Blah %40s!\n", str.c_str()));
    BM(, std::sprintf(dest, "blah %d blah %d blah", 123456, 0x123456));
    BM(, std::sprintf(dest, "blah %+d blah %#x blah", 123456, 0x123456));
    BM(, std::sprintf(dest, "blah %+20d blah %#-20x blah", 123456, 0x123456));
    BM(, std::sprintf(dest, "%g %g %g", 1.123e+5, pi, 1.11e-222));
    BM(, std::sprintf(dest, "%#f %+.8f %30e", 1.123e+5, pi, 1.11e-222));

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    strf::to(stdout)
        ( "\n Variables definitions :"
          "\n    constexpr std::size dest_size = ", dest_size, ';',
          "\n    char dest[dest_size];"
          "\n    constexpr double pi = M_PI;"
          "\n    " STR(FIXTURE_STR)
          "\n"
          "\n `hybrid_fmt_strf(args...)` is equivalent to:"
          "\n    strf::cstr_writer dst{dest, dest_size};"
          "\n    fmt::format_ot(strf::make_iterator(dst), args...);"
          "\n    dst.finish();"
          "\n" );

    return 0;
}
