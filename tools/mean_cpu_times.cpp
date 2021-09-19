#include <fstream>
#include <string>
#include <cstdlib>
#include <numeric>
#include <string_view>
#include <span>
#include <vector>
#include <cassert>
#include <cmath>
#include <limits>

#include <strf/to_cfile.hpp>

auto print_err = strf::to(stderr);

struct parsed_header {
    std::size_t names_field_size = 0;
    std::size_t cpu_begin = 0;
    std::size_t cpu_end = 0;
};

bool is_header(std::string_view line, parsed_header& hdr) {
    if ( ! line.starts_with("Benchmark "))
        return false;
    auto time_pos = line.find_first_not_of(' ', 10);
    if (std::string_view::npos == time_pos) {
        return false;
    }
    if (line.substr(time_pos, 4) != "Time") {
        return false;
    }
    auto cpu_pos = line.find_first_not_of(' ', time_pos + 4);
    if (std::string_view::npos == cpu_pos) {
        return false;
    }
    if (line.substr(cpu_pos, 3) != "CPU") {
        return false;
    }
    auto iterations_pos = line.find_first_not_of(' ', cpu_pos + 3);
    if (std::string_view::npos == iterations_pos) {
        return false;
    }
    if (line.substr(iterations_pos, 10) != "Iterations") {
        return false;
    }
    hdr.names_field_size = time_pos - 12;
    hdr.cpu_begin = time_pos + 5;
    hdr.cpu_end = cpu_pos + 3;
    return true;
}

struct parsed_line {
    std::string benchmark_name;
    double cpu_time = 0;
};

bool parse_line(std::string_view line, parsed_header hdr, parsed_line& result) {
    if (line.size() <= hdr.cpu_end) {
        return false;
    }
    if (line.substr(hdr.cpu_end - 3, 3) != " ns") {
        return false;
    }
    auto cpu_pos = line.find_first_not_of(' ', hdr.cpu_begin);
    if (cpu_pos >= hdr.cpu_end) {
        return false;
    }
    auto name_last_char_pos = line.find_last_not_of(' ', hdr.names_field_size);
    if (std::string_view::npos ==  name_last_char_pos) {
        return false;
    }
    result.benchmark_name = line.substr(0, name_last_char_pos + 1);
    result.cpu_time = atof(&line[cpu_pos]);
    return true;
}

std::vector<parsed_line> parse_file(const char* filename) {
    std::fstream file(filename);
    if (! file.good()) {
        print_err("Could not open file ", filename, '\n');
        return {};
    }
    std::string line;
    parsed_header header;
    while (file.good()) {
        std::getline(file, line);
        if (is_header(line, header)) {
            break;
        }
    }
    std::vector<parsed_line> benchmarks;
    while (file.good()) {
        std::getline(file, line);
        parsed_line line_results;
        if (parse_line(line, header, line_results)) {
            benchmarks.emplace_back(std::move(line_results));
        }
    }
    return benchmarks;
}


std::vector<std::vector<parsed_line>> parse_files(int argc, char* argv[]) {
    if (argc <= 1) {
        print_err("missing arguments ( the file names )\n");
        return {};
    }
    std::vector<std::vector<parsed_line>> parsed_files;
    for (int i = 1; i < argc; ++i) {
        auto r = parse_file(argv[i]);
        if (!r.empty()) {
            parsed_files.emplace_back(std::move(r));
        }
    }
    return parsed_files;
}

bool results_are_consistent(std::vector<std::vector<parsed_line>>& results) {
    assert(!results.empty());

    std::size_t lines_count = results[0].size();
    for (std::size_t i = 1; i < results.size(); ++i) {
        if (results[i].size() != lines_count) {
            print_err("Files have different number of benchmarks\n");
            return false;
        }
    }
    for(std::size_t line = 0; line < lines_count; ++line) {
        std::string_view name = results[0][line].benchmark_name;
        for (std::size_t i = 1; i < results.size(); ++i) {
            if (results[i][line].benchmark_name != name) {
                print_err("Files have different benchmark names\n");
                return false;
            }
        }
    }
    return true;
}

struct statistics {
    double mean;
    double std_deviation;
    double minimum;
    double maximum;
};

statistics calc_statistics(std::span<double> values) {
    double sum = 0;
    double minimum = (std::numeric_limits<double>::max)();
    double maximum = (std::numeric_limits<double>::min)();
    for (auto x : values) {
        sum += x;
        if (x > maximum) {
            maximum = x;
        }
        if (x < minimum) {
            minimum = x;
        }
    }
    const double num_values_d = static_cast<double>(values.size());
    const double mean = sum / num_values_d;
    double devi = 0;
    for (auto x : values) {
        auto diff = x - mean;
        devi += diff * diff;
    }
    devi = std::sqrt(devi / num_values_d);
    return {mean, devi, minimum, maximum};
}

int main(int argc, char* argv[]) {
    auto parsed_files = parse_files(argc, argv);
    if (parsed_files.empty()) {
        return 1;
    }
    if (!results_are_consistent(parsed_files)) {
        return 1;
    }
    auto print = strf::to(stdout);
    std::size_t lines_count = parsed_files[0].size();
    std::vector<double> cpu_times(parsed_files.size());

    print( strf::right("MEAN", 20)
         , strf::right("MIN", 18)
         , strf::right("MAX", 15)
         , strf::right("AMPLITUDE", 15)
         , strf::right("STD.DEVIATION", 15)
         , '\n' );

    for (std::size_t line = 0; line < lines_count; ++line) {
        for (std::size_t i = 0; i < parsed_files.size(); ++i) {
            cpu_times[i] = parsed_files[i][line].cpu_time;
        }
        auto r = calc_statistics(cpu_times);
        print( strf::right(line + 1, 3), ") "
             , strf::gen(r.mean, 3) > 15, " ns"
             , strf::fixed(r.minimum) > 15
             , strf::fixed(r.maximum) > 15
             , strf::gen(r.maximum - r.minimum, 3) > 15
             , strf::fixed(r.std_deviation, 3) > 15
             , '\n');
    }
    print("\n\n");
    for (std::size_t line = 0; line < lines_count; ++line) {
        print(line + 1, ") ", parsed_files[0][line].benchmark_name, '\n');
        for (std::size_t i = 0; i < parsed_files.size(); ++i) {
            double cpu_time = parsed_files[i][line].cpu_time;
            print(strf::fixed(cpu_time) > 10, '\n');
        }
        print('\n');
    }
    return 0;
}
