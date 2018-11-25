#ifndef RANDOMGEN_H
#define RANDOMGEN_H

#include "alglib/ap.h"

#include <memory>
#include <random>
#include <functional>
#include <any>
#include <vector>
#include <variant>
#include <tuple>
#include <map>

namespace randomgen {

  // Interface
  using RandomEngine = std::default_random_engine;
  using SeedType = RandomEngine::result_type;

  struct TRandomNumberGenerator;
  using PRandomNumberGenerator = std::unique_ptr<randomgen::TRandomNumberGenerator>;

  using ParamsTwoVectors = std::tuple<const std::vector<double> &, const std::vector<double> &>;
  using CreateGeneratorParams = std::variant<double, ParamsTwoVectors>;

  struct TRandomNumberGenerator {
    virtual ~TRandomNumberGenerator();

    virtual double
    Generate() const = 0;

  protected:
    TRandomNumberGenerator(SeedType);

    mutable RandomEngine engine;
  };

  // Factory
  using CreateGeneratorFunction = std::function<PRandomNumberGenerator(const CreateGeneratorParams &, SeedType)>;
  struct TRandomNumberGeneratorFactory {
    static std::shared_ptr<TRandomNumberGeneratorFactory>
    getSingleton();

    static void
    registerGenerator(const std::string &, const CreateGeneratorFunction &);

    static PRandomNumberGenerator
    createGenerator(const std::string &, const CreateGeneratorParams &, SeedType = RandomEngine::default_seed);

  private:
    TRandomNumberGeneratorFactory();

    static std::shared_ptr<TRandomNumberGeneratorFactory> singleton;

    std::map<std::string, CreateGeneratorFunction> generatorMap;
  };

  // Bernoulli
  struct TBernoulliGenerator : public TRandomNumberGenerator {
    TBernoulliGenerator(double, SeedType);

    virtual double
    Generate() const;

    static PRandomNumberGenerator
    createGenerator(const CreateGeneratorParams &, SeedType);
  private:
    RandomEngine::result_type p;
  };

  // Finite
  struct TFiniteGenerator : public TRandomNumberGenerator {
    TFiniteGenerator(const std::vector<double> &, const std::vector<double> &, SeedType);

    virtual double
    Generate() const;

    static PRandomNumberGenerator
    createGenerator(const CreateGeneratorParams &, SeedType);
  private:
    std::vector<double> x;
    std::vector<RandomEngine::result_type> p;
  };

  // Geometic
  struct TGeometricGenerator : public TRandomNumberGenerator {
    TGeometricGenerator(double, SeedType);

    virtual double
    Generate() const;

    static PRandomNumberGenerator
    createGenerator(const CreateGeneratorParams &, SeedType);
  private:
    long double p;
  };

  // Poisson
  struct TPoissonGenerator : public TRandomNumberGenerator {
    TPoissonGenerator(double, SeedType);

    virtual double
    Generate() const;

    static PRandomNumberGenerator
    createGenerator(const CreateGeneratorParams &, SeedType);
  private:
    alglib::ae_int_t k;
  };
}

#endif
