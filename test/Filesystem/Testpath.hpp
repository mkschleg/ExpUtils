
#define _TEST_ 1

#include "Filesystem.h"
#include "gtest/gtest.h"

enum class Type : unsigned char {
  multi, rootName, rootDir, filename, dir
};


TEST(PathCreate, lvalueSingle){
  std::string s = "path.txt";
  ExpUtils::Filesystem::path p(s);
  EXPECT_EQ(p.string(), s);
}

TEST(PathCreate, lvalueMulti){
  std::string s = "test/dir/path.txt";
  ExpUtils::Filesystem::path p(s);
  EXPECT_EQ(p.string(), s);
}

TEST(PathCreate, rvalueSingle){
  std::string s = "path.txt";
  ExpUtils::Filesystem::path p(std::move(s));
  EXPECT_TRUE(s.empty());
  EXPECT_NE(p.string(), s);
  EXPECT_EQ(p.string(), "path.txt");
}

TEST(PathCreate, rvalueMulti){
  std::string s = "test/dir/path.txt";
  ExpUtils::Filesystem::path p(std::move(s));
  EXPECT_TRUE(s.empty());
  EXPECT_NE(p.string(), s);
  EXPECT_EQ(p.string(), "test/dir/path.txt");

}

TEST(PathCMPTS, lvalue){
  std::string s = "/test/dir/path.txt";
  ExpUtils::Filesystem::path p(s);
  std::vector<ExpUtils::Filesystem::path::cmpt> cmpts = p.get_cmpts();
  //Check to make sure cmpts have been created.
  ASSERT_EQ(cmpts.size(), 4);

  EXPECT_EQ(cmpts[0].string(), "/");
  EXPECT_EQ(cmpts[0].get_type(), (unsigned char)Type::rootDir);

  // ASSERT_EQ(cmpts.size(), 4);
  EXPECT_EQ(cmpts[1].string(), "test");
  EXPECT_EQ(cmpts[1].get_type(), (unsigned char)Type::dir);

  // ASSERT_EQ(cmpts.size(), 4);
  EXPECT_EQ(cmpts[2].string(), "dir");
  EXPECT_EQ(cmpts[2].get_type(), (unsigned char)Type::dir);

  EXPECT_EQ(cmpts[3].string(), "path.txt");
  EXPECT_EQ(cmpts[3].get_type(), (unsigned char)Type::filename);
}

TEST(PathCreate, IteratorSingle){
  std::string s = "path.txt";
  ExpUtils::Filesystem::path p(s.begin(), s.end());
  EXPECT_EQ(p.string(), s);
}

TEST(PathCreate, IteratorMulti){
  std::string s = "test/dir/path.txt";
  ExpUtils::Filesystem::path p(s.begin(), s.end());
  EXPECT_EQ(p.string(), s);
}


TEST(PathOperator, concatPath){
  std::string s1 = "dir/";
  std::string s2 = "foo.txt";
  ExpUtils::Filesystem::path p(s1);
  p += ExpUtils::Filesystem::path(s2);
  EXPECT_EQ(p.string(), (s1+s2));
}

TEST(PathOperator, concatString){
  std::string s1 = "dir/";
  std::string s2 = "foo.txt";
  ExpUtils::Filesystem::path p(s1);
  p += s2;
  EXPECT_EQ(p.string(), (s1+s2));
}

TEST(PathOperator, appendPath){
  std::string s1 = "dir";
  std::string s2 = "foo.txt";
  ExpUtils::Filesystem::path p(s1);
  p /= ExpUtils::Filesystem::path(s2);
  EXPECT_EQ(p.string(), (s1+"/"+s2));
}

TEST(PathOperator, appendString){
  std::string s1 = "dir";
  std::string s2 = "foo.txt";
  ExpUtils::Filesystem::path p(s1);
  p /= s2;
  EXPECT_EQ(p.string(), (s1+"/"+s2));
}

TEST(PathOperator, assignment){
  ExpUtils::Filesystem::path p1("dir/helloWorld.txt");
  ExpUtils::Filesystem::path p2("test/dir/helloWorld2.txt");
  p1 = std::move(p2);
  EXPECT_TRUE(p2.empty());
  EXPECT_EQ(p1.string(), "test/dir/helloWorld2.txt");
  // EXPECT_STREQ(s1, s2)
  // p /= s2;
  // EXPECT_STREQ(p.string().c_str(), (s1+"/"+s2).c_str());
}


TEST(PathDecomp, exstention){
  ExpUtils::Filesystem::path p("dir/helloWorld.txt.tmp");
  EXPECT_EQ(p.extension().string(), ".tmp");
  ExpUtils::Filesystem::path p1("dir/..");
  EXPECT_EQ(p1.extension().string(), "");
  ExpUtils::Filesystem::path p2("dir/.");
  EXPECT_EQ(p2.extension().string(), "");

  ExpUtils::Filesystem::path p3("");
  EXPECT_EQ(p3.extension().string(), "");

}

TEST(PathDecomp, stem){
  ExpUtils::Filesystem::path p("dir/helloWorld.txt.tmp");
  EXPECT_EQ(p.stem().string(), "helloWorld.txt");
  EXPECT_EQ(p.stem().stem().string(), "helloWorld");
  ExpUtils::Filesystem::path p1("dir/");
  EXPECT_EQ(p1.stem().string(), ".");
  EXPECT_EQ(p1.stem().stem().string(), ".");
  ExpUtils::Filesystem::path p2("dir/..");
  EXPECT_EQ(p2.stem().string(), "..");
  EXPECT_EQ(p2.stem().stem().string(), "..");
  ExpUtils::Filesystem::path p3("");
  EXPECT_EQ(p3.stem().string(), "");
}
