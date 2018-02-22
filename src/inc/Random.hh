#pragma once

#include <random>
#include <unistd.h>

namespace ExpUtils {
namespace Random {

namespace {
// std::minstd_rand generator{[&](){return getpid();}};
inline std::default_random_engine &getGeneratorInstance() {
  static std::default_random_engine instance{(uint)getpid()};
  return instance;
}
} // namespace
//
//
inline void seed(int seed) {
  if (seed == 0) {
    seed = getpid();
  }
  getGeneratorInstance().seed(seed);
}

/// \brief Random integer in range [0,max)
inline int range(int max) {
  static std::uniform_int_distribution<int> dist{};
  return dist(getGeneratorInstance(), decltype(dist)::param_type{0, max - 1});
}

/// \brief Random integer in range [min,max]
inline int range(int min, int max) {
  static std::uniform_int_distribution<int> dist{};
  return dist(getGeneratorInstance(), decltype(dist)::param_type{min, max});
}

/// \brief Random number from the uniform distribution
inline double uniform() {
  static std::uniform_real_distribution<double> dist{0, 1};
  return dist(getGeneratorInstance());
}

/// \brief Random number from the Normal Distribuion
inline double normal(double mu, double sigma) {
  static std::normal_distribution<double> dist{};
  using param_t = decltype(dist)::param_type;
  return dist(getGeneratorInstance(), param_t{mu, sigma});
}

} // namespace Random
} // namespace ExpUtils
