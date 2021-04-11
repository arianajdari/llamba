#include <llamba/base/multiply_parallel.hpp>
#include <llamba/base/sum_parallel.hpp>
#include <llamba/base/complex_parallel.hpp>
#include <llamba/base/grayscale_parallel.hpp>

#include <iostream>

#include <llamba/benchmark/benchmark.hpp>
#include <llamba/generators/generator.hpp>
// #include <llamba/base/adjoint_parallel.hpp>

#include <llamba/base/inverse_parallel.hpp>


int main()
{
  auto matrices_a = llamba::generator::generate_inputs<double>({40});
  auto matrices_b = llamba::generator::generate_inputs_zero<double>({40});

  

  llamba::settings input_settings_one   = llamba::settings(2_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 19_PRIORITY, AFFINITY::FALSE, PARALLELIZATION_STRATEGY::PTHREADS);
  llamba::settings input_settings_two   = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 21_PRIORITY, AFFINITY::TRUE, std::vector<int>{0, 2, 4, 6}, PARALLELIZATION_STRATEGY::PTHREADS);
  llamba::settings input_settings_three = llamba::settings(6_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 45_PRIORITY, AFFINITY::TRUE, std::vector<int>{0, 2, 4, 6}, PARALLELIZATION_STRATEGY::PTHREADS);
  llamba::settings input_settings_four  = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::OPENMP);
  llamba::settings input_settings_five  = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);
 
  llamba::benchmark<double> benchmark_adjoint_parallel(new llamba::base::multiply_parallel<double>(), matrices_a, matrices_b, std::vector<llamba::settings>{input_settings_one, input_settings_two, input_settings_three, input_settings_four, input_settings_five});

  benchmark_adjoint_parallel.execute_all<TIME_FORMAT::NANO>();

  return 0;
}

