//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <strf/to_cfile.hpp>
#include <strf/to_string.hpp>

#include <benchmark/benchmark.h>
#include <cstdio>
#include <climits>
#include <clocale>


#ifndef __cpp_char8_t
using char8_t = char;
#endif

char8_t u8sample1_[] =
    u8"ABCDEFGHIJLIMOPQRSTUVWXYZabcdefghijlimopqrstuvwxyz"
    u8"012345678901234567890123456789";

char8_t u8sample2_[] =
    u8"\u0080\u0081\u0082\u0083\u0084\u0085\u0086\u0087"
    u8"\u0088\u0089\u008A\u008B\u008C\u008D\u008E\u008F"
    u8"\u0100\u0101\u0102\u0103\u0104\u0105\u0106\u0107"
    u8"\u0108\u0109\u010A\u010B\u010C\u010D\u010E\u010F"
    u8"\u0110\u0111\u0112\u0113\u0114\u0115\u0116\u0117"
    u8"\u0118\u0119\u011A\u011B\u011C\u011D\u011E\u011F"
    u8"\u0120\u0121\u0122\u0123\u0124\u0125\u0126\u0127"
    u8"\u0128\u0129\u012A\u012B\u012C\u012D\u012E\u012F"
    u8"\u0130\u0131\u0132\u0133\u0134\u0135\u0136\u0137"
    u8"\u0138\u0139\u013A\u013B\u013C\u013D\u013E\u013F"
    u8"\u0140\u0141\u0142\u0143\u0144\u0145\u0146\u0147"
    u8"\u0148\u0149\u014A\u014B\u014C\u014D\u014E\u014F";

char8_t u8sample3a_[] =
    u8"\u0800\u0801\u0802\u0803\u0804\u0805\u0806\u0807"
    u8"\u0808\u0809\u080A\u080B\u080C\u080D\u080E\u080F"
    u8"\u0810\u0811\u0812\u0813\u0814\u0815\u0816\u0817"
    u8"\u0818\u0819\u081A\u081B\u081C\u081D\u081E\u081F"
    u8"\u0820\u0821\u0822\u0823\u0824\u0825\u0826\u0827"
    u8"\u0828\u0829\u082A\u082B\u082C\u082D\u082E\u082F"
    u8"\u0830\u0831\u0832\u0833\u0834\u0835\u0836\u0837"
    u8"\u0838\u0839\u083A\u083B\u083C\u083D\u083E\u083F"
    u8"\u0840\u0841\u0842\u0843\u0844\u0845\u0846\u0847"
    u8"\u0848\u0849\u084A\u084B\u084C\u084D\u084E\u084F";

char8_t u8sample3b_[] =
    u8"\uF000\uF001\uF002\uF003\uF004\uF005\uF006\uF007"
    u8"\uF008\uF009\uF00A\uF00B\uF00C\uF00D\uF00E\uF00F"
    u8"\uF010\uF011\uF012\uF013\uF014\uF015\uF016\uF017"
    u8"\uF018\uF019\uF01A\uF01B\uF01C\uF01D\uF01E\uF01F"
    u8"\uF020\uF021\uF022\uF023\uF024\uF025\uF026\uF027"
    u8"\uF028\uF029\uF02A\uF02B\uF02C\uF02D\uF02E\uF02F"
    u8"\uF030\uF031\uF032\uF033\uF034\uF035\uF036\uF037"
    u8"\uF038\uF039\uF03A\uF03B\uF03C\uF03D\uF03E\uF03F"
    u8"\uF040\uF041\uF042\uF043\uF044\uF045\uF046\uF047"
    u8"\uF048\uF049\uF04A\uF04B\uF04C\uF04D\uF04E\uF04F";


char8_t u8sample4_[] =
    u8"\U00010000\U00010001\U00010002\U00010003\U00010004\U00010005\U00010006\U00010007"
    u8"\U00010008\U00010009\U0001000A\U0001000B\U0001000C\U0001000D\U0001000E\U0001000F"
    u8"\U00010010\U00010011\U00010012\U00010013\U00010014\U00010015\U00010016\U00010017"
    u8"\U00010018\U00010019\U0001001A\U0001001B\U0001001C\U0001001D\U0001001E\U0001001F"
    u8"\U00010020\U00010021\U00010022\U00010023\U00010024\U00010025\U00010026\U00010027"
    u8"\U00010028\U00010029\U0001002A\U0001002B\U0001002C\U0001002D\U0001002E\U0001002F"
    u8"\U00010030\U00010031\U00010032\U00010033\U00010034\U00010035\U00010036\U00010037"
    u8"\U00010038\U00010039\U0001003A\U0001003B\U0001003C\U0001003D\U0001003E\U0001003F"
    u8"\U00010040\U00010041\U00010042\U00010043\U00010044\U00010045\U00010046\U00010047"
    u8"\U00010048\U00010049\U0001004A\U0001004B\U0001004C\U0001004D\U0001004E\U0001004F";

char32_t u32buff[100];

strf::detail::simple_string_view<char8_t> u8sample1{u8sample1_};
strf::detail::simple_string_view<char8_t> u8sample2{u8sample2_};
strf::detail::simple_string_view<char8_t> u8sample3a{u8sample3a_};
strf::detail::simple_string_view<char8_t> u8sample3b{u8sample3b_};
strf::detail::simple_string_view<char8_t> u8sample4{u8sample4_};


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
                auto r = EXPR ;                                         \
                benchmark::DoNotOptimize(u32buff);                      \
                benchmark::DoNotOptimize(r);                      \
                benchmark::ClobberMemory();                             \
            }                                                           \
        }                                                               \
    };                                                                  \
    benchmark::RegisterBenchmark(LABEL, ID :: func);

int main(int argc, char** argv)
{
    BM(, strf::to(u32buff) (strf::sani(u8sample1)));
    BM(, strf::to(u32buff) (strf::sani(u8sample2)));
    BM(, strf::to(u32buff) (strf::sani(u8sample3a)));
    BM(, strf::to(u32buff) (strf::sani(u8sample3b)));
    BM(, strf::to(u32buff) (strf::sani(u8sample4)));

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}

