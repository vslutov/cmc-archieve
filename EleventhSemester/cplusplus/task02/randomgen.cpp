#include "randomgen.h"

#include "alglib/specialfunctions.h"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>

namespace randomgen {
  static double EPS = 1e-5;

  TRandomNumberGenerator::~TRandomNumberGenerator() = default;

  TRandomNumberGenerator::TRandomNumberGenerator(SeedType seed) :
    engine(seed)
  {
  }

  // Factory
  TRandomNumberGeneratorFactory::TRandomNumberGeneratorFactory() :
    generatorMap()
  {
  };
  std::shared_ptr<TRandomNumberGeneratorFactory> TRandomNumberGeneratorFactory::singleton;

  std::shared_ptr<TRandomNumberGeneratorFactory>
  TRandomNumberGeneratorFactory::getSingleton() {
    if (singleton.get() == nullptr) {
      singleton = std::shared_ptr<TRandomNumberGeneratorFactory>(new TRandomNumberGeneratorFactory());
    }
    return singleton;
  }

  void
  TRandomNumberGeneratorFactory::registerGenerator(const std::string &name, const CreateGeneratorFunction &func)
  {
    getSingleton()->generatorMap[name] = func;
  }

  PRandomNumberGenerator
  TRandomNumberGeneratorFactory::createGenerator(const std::string &name, const CreateGeneratorParams &params, SeedType seed) {
    const auto &generatorMap = getSingleton()->generatorMap;

    if (generatorMap.count(name) == 0) {
      return PRandomNumberGenerator(nullptr);
    }

    CreateGeneratorFunction createGeneratorFunction = getSingleton()->generatorMap[name];
    return createGeneratorFunction(params, seed);
  }

  // Bernoulli
  TBernoulliGenerator::TBernoulliGenerator(double _p, SeedType seed) :
    TRandomNumberGenerator(seed),
    p(_p * (static_cast<long double>(RandomEngine::max()) - RandomEngine::min()) + RandomEngine::min())
  {
  }

  double
  TBernoulliGenerator::Generate() const
  {
    return this->engine() <= this->p;
  }

  PRandomNumberGenerator
  TBernoulliGenerator::createGenerator(const CreateGeneratorParams &params, SeedType seed) {
    auto p = std::get_if<double>(&params);
    if (p == nullptr || (*p < -EPS) || (*p > 1 + EPS)) {
      return PRandomNumberGenerator(nullptr);
    }

    return PRandomNumberGenerator(new TBernoulliGenerator(*p, seed));
  }

  // Finite
  TFiniteGenerator::TFiniteGenerator(const std::vector<double> &_x, const std::vector<double> &_p, SeedType seed) :
    TRandomNumberGenerator(seed),
    x(_x),
    p(_p.size())
  {
    auto len = static_cast<long double>(RandomEngine::max()) - RandomEngine::min();
    auto partitial = std::vector<long double>(_p.size());
    std::partial_sum(_p.begin(), _p.end(), partitial.begin());

    ssize_t len_p = _p.size();
    for (ssize_t i = 0; i < len_p; ++ i) {
      p[i] = partitial[i] * len + RandomEngine::min();
    }
  }

  double
  TFiniteGenerator::Generate() const
  {
    double gen_result = this->engine();
    ssize_t p_len = p.size();
    for (ssize_t i = 0; i < p_len; ++ i) {
      if (gen_result <= p[i]) {
        return x[i];
      }
    }

    return *(x.rbegin());
  }

  PRandomNumberGenerator
  TFiniteGenerator::createGenerator(const CreateGeneratorParams &params, SeedType seed) {
    auto xp = std::get_if<ParamsTwoVectors>(&params);
    if (xp == nullptr) {
      return PRandomNumberGenerator(nullptr);
    }

    std::vector<double> x;
    std::vector<double> p;
    std::tie(x, p) = *xp;


    if ((p.size() == x.size()) &&
        (std::abs(std::accumulate(p.begin(), p.end(), 0.0) - 1.0) <= EPS) &&
        std::all_of(p.begin(), p.end(), [](double p_now) {
          return (p_now >= -EPS) && (p_now <= 1 + EPS);
        })) {
      return PRandomNumberGenerator(new TFiniteGenerator(x, p, seed));
    } else {
      return PRandomNumberGenerator(nullptr);
    }
  }

  // Geometric
  TGeometricGenerator::TGeometricGenerator(double _p, SeedType seed) :
    TRandomNumberGenerator(seed),
    p(_p)
  {
  }

  double
  TGeometricGenerator::Generate() const
  {
    auto gen_result = (this->engine() - static_cast<long double>(RandomEngine::min())) / (static_cast<long double>(RandomEngine::max()) - RandomEngine::min());

    return std::floor(std::log(gen_result) / std::log(1 - p));
  }

  PRandomNumberGenerator
  TGeometricGenerator::createGenerator(const CreateGeneratorParams &params, SeedType seed) {
    auto p = std::get_if<double>(&params);
    if (p == nullptr) {
      return PRandomNumberGenerator(nullptr);
    }

    if ((*p >= 0 - EPS) && (*p <= 1 + EPS)) {
      return PRandomNumberGenerator(new TGeometricGenerator(*p, seed));
    } else {
      return PRandomNumberGenerator(nullptr);
    }
  }

  // Poisson
  TPoissonGenerator::TPoissonGenerator(double _k, SeedType seed) :
    TRandomNumberGenerator(seed),
    k(_k)
  {
  }

  double
  TPoissonGenerator::Generate() const
  {
    auto gen_result = (this->engine() - static_cast<long double>(RandomEngine::min())) / (static_cast<long double>(RandomEngine::max()) - RandomEngine::min());

    return alglib::invpoissondistribution(k, gen_result) - 1;
  }

  PRandomNumberGenerator
  TPoissonGenerator::createGenerator(const CreateGeneratorParams &params, SeedType seed) {
    auto k = std::get_if<double>(&params);
    if (k == nullptr || *k <= EPS) {
      return PRandomNumberGenerator(nullptr);
    }

    return PRandomNumberGenerator(new TPoissonGenerator(*k, seed));
  }
}
