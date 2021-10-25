#include <vector>

#include <llamba/generators/generator.hpp>
#include <llamba/base/sum_parallel.hpp>

#include <llamba/benchmark/benchmark.hpp>

#define DATA_TYPE double

int main()
{
    auto matrices_a = llamba::generator::generate_inputs<DATA_TYPE>({100, 200});
    auto matrices_b = llamba::generator::generate_inputs<DATA_TYPE>({100, 200});

    llamba::settings input_settings_one = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings input_settings_two = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);

    llamba::benchmark<DATA_TYPE> benchmark_summation(
        new llamba::base::sum_parallel<DATA_TYPE>(),
        matrices_a, matrices_b,
        std::vector<llamba::settings>{input_settings_one, input_settings_two}
    );

    benchmark_summation.execute_all<TIME_FORMAT::MILLI>();

    return 0;
}