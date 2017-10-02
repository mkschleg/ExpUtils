

#include "gtest/gtest.h"
#include "Parser.h"

TEST(ParserTest, vectorTest){
  double d1, d2;
  int i1, i2;
  std::vector<std::string> slist{{"1.123","3.14159","293","550"}};
  ExpUtils::ParseList(slist, d1, d2, i1, i2);
  
  EXPECT_EQ(d1, 1.123);
  EXPECT_EQ(d2, 3.14159);
  EXPECT_EQ(i1, 293);
  EXPECT_EQ(i2, 550);


}


TEST(ParserTest, commandLineTestTest){
  double d1, d2;
  int i1, i2;
  // std::vector<std::string> slist{{"1.123","3.14159","293","550"}};
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wwritable-strings"
  char* argv[] = {"Hello World Func","1.123","3.14159","293","550"};
  #pragma clang diagnostic pop
  int argc = 5;
  ExpUtils::ParseList(std::vector<std::string>(argv+1, argv+argc), d1, d2, i1, i2);

  EXPECT_EQ(d1, 1.123);
  EXPECT_EQ(d2, 3.14159);
  EXPECT_EQ(i1, 293);
  EXPECT_EQ(i2, 550);


}

TEST(ParserTest, tupleTest){

  std::vector<std::string> slist{{"1.123","3.14159","293","550"}};
  auto ret = ExpUtils::ParseList<double,double,int,int>(slist);

  EXPECT_EQ(std::get<0>(ret), 1.123);
  EXPECT_EQ(std::get<1>(ret), 3.14159);
  EXPECT_EQ(std::get<2>(ret), 293);
  EXPECT_EQ(std::get<3>(ret), 550);


}

TEST(ParserTest, exceptionTest){
  std::vector<std::string> slist{{"1.123","s3.14159","293","550"}};
  bool exceptionFired = false;
  try{
    ExpUtils::ParseList<double, double, int, int>(slist);
  } catch(...){
    exceptionFired = true;
  }
  EXPECT_TRUE(exceptionFired);
  exceptionFired = false;
  try{
    ExpUtils::ParseList<double,double,int,int,int,int>(slist);
  } catch(...){
    exceptionFired = true;
  }
}
