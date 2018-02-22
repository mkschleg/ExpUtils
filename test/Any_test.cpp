

#include "gtest/gtest.h"
// #include "Any.hh"
#include "ExpUtils"
#include <iostream>
#include <string>


using ExpUtils::Any;
TEST(AnyTest, value){
  Any a(1.0);
  // std::cout<<a.retrieve<int>()<<std::endl;
  EXPECT_TRUE(a == 1.0 && 1.0 == a);
  EXPECT_EQ(a.retrieve<double>(),1.0);
  a = 5;
  EXPECT_TRUE(5 == a && a == 5);
  EXPECT_EQ(a.retrieve<int>(), 5);
  a = std::string("Hello World");
  // std::cout<<a.retrieve<int>()<<std::endl;
  // std::cout<<a.retrieve<std::string>()<<std::endl;
  EXPECT_TRUE(a == std::string("Hello World") && std::string("Hello World") == a);
  EXPECT_EQ(a.retrieve<std::string>(), "Hello World");
}

TEST(AnyTest, type){
  Any a(1.0);
  EXPECT_EQ(a.getType(),typeid(double));
}
