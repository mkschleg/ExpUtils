// #include "Random.h"

#include <random>
#include <unistd.h>


namespace ExpUtils{
namespace Random{


  // namespace{
  //   // std::minstd_rand generator{[&](){return getpid();}};
  //   std::default_random_engine& getGeneratorInstance(){
  //     static std::default_random_engine instance{(uint)getpid()};
  //     return instance;
  //   }
  // }
  // 
  // 
  // void seed(int seed){
  //   if(seed == 0){
  //     seed = getpid();
  //   }
  //   getGeneratorInstance().seed(seed);
  // }
  // 
  // int range(int max){
  //   static std::uniform_int_distribution<int> dist{};
  //   return dist(getGeneratorInstance(), decltype(dist)::param_type{0,max-1});
  // }
  // 
  // double uniform(){
  //   static std::uniform_real_distribution<double> dist{0,1};
  //   return dist(getGeneratorInstance());
  // }
  // 
  // double normal(double mu, double sigma){
  //   static std::normal_distribution<double> dist{};
  //   using param_t = decltype(dist)::param_type;
  //   return dist(getGeneratorInstance(), param_t{mu, sigma});
  // }



}
}
