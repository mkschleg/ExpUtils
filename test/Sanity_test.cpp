

#include "gtest/gtest.h"
#include "Memory.h"
// #include <memory>
#include <iostream>


class A{
public:
  int i;
  A(int j):i(j){}
  A(const A& a):i(a.i){std::cout<<"copy"<<std::endl;}
  A(A&& a):i(a.i){a.i = 0; std::cout<<"Move"<<std::endl;}
};



TEST(Sanity, UniquePtrMoveSemantics){
  
  std::unique_ptr<A> up_A = ExpUtils::Memory::make_unique<A>(10);
  EXPECT_EQ(up_A->i, 10);

  A a(10);
  std::unique_ptr<A> up_A2 = ExpUtils::Memory::make_unique<A>(std::move(a));
  EXPECT_EQ(up_A2->i, 10);
  EXPECT_EQ(a.i, 0);
  
  std::unique_ptr<A> up_A3 = ExpUtils::Memory::make_unique<A>(A(9));
  EXPECT_EQ(up_A3->i, 9);


}
