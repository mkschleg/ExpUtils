#include "gtest/gtest.h"
#include "Experiment.hh"
#include "Memory.hh"
// #include <memory>
//
// class Experiment:public ExpUtils::Experiment<Experiment>{
// public:
//   // int init;
//   int init(int i, int j){
//     std::cout<<"init Derived"<<" "<<i<<" "<<j<<std::endl;
//     return 1;
//   }
//   void reset(int i){}
//   // void start() override{
//   // 
//   // }
//   // void execute() override{
//   // 
//   // }
// 
// };
// 
//
// 
// TEST(Experiment, initial){
// 
//   // Experiment exp;
//   std::unique_ptr<ExpUtils::Experiment<Experiment>> exp = ExpUtils::Memory::make_unique<Experiment>();
//   exp->init(1,2);
//   exp->reset(1);
//   // static_assert(std::is_detected<decltype(&Experiment::init)>::value,
//   //                 "A::member is not a member function.");
// 
// 
// }
