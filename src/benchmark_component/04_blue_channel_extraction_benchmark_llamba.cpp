#include <vector>

#include <llamba/generators/generator.hpp>
#include <llamba/base/blue_channel_parallel.hpp>

#include <llamba/benchmark/benchmark.hpp>

#define DATA_TYPE int

int main()
{
    auto matrices_a = llamba::generator::generate_inputs<DATA_TYPE>({100, 200});
    auto matrices_b = llamba::generator::generate_inputs<DATA_TYPE>({100, 200});

    llamba::settings input_settings_one = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings input_settings_two = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);

    llamba::benchmark<DATA_TYPE> blue_channel_parallel(
        new llamba::base::blue_channel_parallel<DATA_TYPE>(),
        matrices_a, matrices_b,
        std::vector<llamba::settings>{input_settings_one, input_settings_two}
    );

    blue_channel_parallel.execute_all<TIME_FORMAT::MILLI>();

    return 0;
}