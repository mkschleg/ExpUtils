#pragma once


namespace ExpUtils{
namespace Random{
  void seed(int iseed = 0);
  int range(int max);
  double uniform();
  double normal(double mu, double sigma);
}
}
