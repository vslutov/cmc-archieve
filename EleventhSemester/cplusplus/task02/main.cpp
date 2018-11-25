#include "main.h"
#include "randomgen.h"
#include "alglib/statistics.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <cmath>
#include <numeric>

template<class InputIterator>
static auto
mean(InputIterator begin, InputIterator end)
{
  using value_type = typename std::iterator_traits<InputIterator>::value_type;
  return std::accumulate(begin, end, value_type(0)) / (end - begin);
}

template<class InputIterator>
static auto
stddev(InputIterator begin, InputIterator end)
{
  using value_type = typename std::iterator_traits<InputIterator>::value_type;
  value_type mean_sample = mean(begin, end);
  return std::accumulate(begin, end, value_type(0), [mean_sample](value_type sum, value_type x) {
    return sum + (x - mean_sample) * (x - mean_sample);
  }) / (end - begin - 1);
}

static double
calc_mean(randomgen::PRandomNumberGenerator &gen,
          ssize_t count)
{
  double mean = 0;
  for (ssize_t i = 0; i < count; ++ i) {
    mean += gen->Generate();
  }
  return mean / count;
}

static bool
student_test(const std::string distribution_name,
             const randomgen::PRandomNumberGenerator &gen,
             ssize_t count,
             double exp_mean,
             double pvalue)
{
  auto sample = std::vector<double>(count);

  for (auto &x : sample) {
    x = gen->Generate();
  }

  alglib::real_1d_array alglib_distribution;
  alglib_distribution.attach_to_ptr(count, sample.data());

  double lefttail, righttail, bothtails;

  studentttest1(alglib_distribution, count, exp_mean, bothtails, lefttail, righttail);
  std::cout << "Student's test for " << distribution_name << std::endl
            << "Expected mean: " << exp_mean << std::endl
            << "Sample mean: " << mean(sample.begin(), sample.end()) << std::endl
            << "Sample stddev: " << stddev(sample.begin(), sample.end()) << std::endl
            << "P(mean_exp = mean_sample) = " << bothtails << std::endl << std::endl;

  return bothtails >= pvalue;
}

void
test_generator(const std::string &name, const randomgen::CreateGeneratorParams &params, double exp_mean)
{
  auto gen = randomgen::TRandomNumberGeneratorFactory::createGenerator(name, params);

  if (gen == nullptr) {
    throw std::runtime_error(name + " generator haven't created");
  }

  if (!student_test(name, gen, 5000, exp_mean, 0.8)) {
    throw std::runtime_error(name + " generator Student's test failed");
  }
}

int main()
{
  std::ios::sync_with_stdio(false);

  randomgen::TRandomNumberGeneratorFactory::registerGenerator("bernoulli", randomgen::TBernoulliGenerator::createGenerator);
  randomgen::TRandomNumberGeneratorFactory::registerGenerator("finite", randomgen::TFiniteGenerator::createGenerator);
  randomgen::TRandomNumberGeneratorFactory::registerGenerator("geometric", randomgen::TGeometricGenerator::createGenerator);
  randomgen::TRandomNumberGeneratorFactory::registerGenerator("poisson", randomgen::TPoissonGenerator::createGenerator);

  try {

    test_generator("bernoulli", 0.2, 0.2);

    std::vector<double> vx = {0, 1, 2};
    std::vector<double> vp = {0.25, 0.25, 0.5};
    test_generator("finite", std::tie(vx, vp), 1.25);

    test_generator("geometric", 0.2, 4);

    test_generator("poisson", 20.0, 20);

  } catch (std::exception &exc) {
    std::cerr << "  " << exc.what() << std::endl;
    return 1;
  } catch (...) {
    return 1;
  }

  return 0;
}
