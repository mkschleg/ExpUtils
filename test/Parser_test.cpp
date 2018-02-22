

#include "gtest/gtest.h"
#include "ExpUtils"

// #include

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

TEST(ConvertTest, convertToVector){

  std::string str = "1 2 3 4 5 6";
  std::vector<int> v(6);
  EXPECT_TRUE(ExpUtils::convert(str, v));

  for( int i = 1; i < 7; i++ ){
    EXPECT_EQ(v[i-1],i);
  }
}


TEST(ConvertTest, convertToArray){

  std::string str = "1 2 3 4 5 6";
  std::array<int,6> v;
  EXPECT_TRUE(ExpUtils::convert(str, v));

  for( int i = 1; i < 7; i++ ){
    EXPECT_EQ(v[i-1],i);
  }
  
  str = "1 2 3 4";
  EXPECT_FALSE(ExpUtils::convert(str, v));
  
  for( int i = 1; i < 7; i++ ){
    EXPECT_EQ(v[i-1],i);
  }

}

TEST(ConvertTest, convertToBool){

  std::string str = "1";
  bool c = false;
  EXPECT_TRUE(ExpUtils::convert(str,c));
  EXPECT_TRUE(c);
  
  str = "0";
  EXPECT_TRUE(ExpUtils::convert(str,c));
  EXPECT_FALSE(c);

}
