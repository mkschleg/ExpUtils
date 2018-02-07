
#define _TEST_ 1

#include "gtest/gtest.h"
#include "Filesystem.hh"


TEST(FilesystemTest, DirectoryCreate){
  std::string dir = "DirectoryCreate/";

  //Check for the existance of a directory that is not there.
  struct stat statbuf;
  bool ret = false;
  // int rt = lstat(dir.c_str(), &st);
  if(lstat(dir.c_str(), &statbuf) != -1){
    if(S_ISDIR(statbuf.st_mode)){
      ret = true;
    }
  }
  
  EXPECT_FALSE(ret);

  EXPECT_TRUE(ExpUtils::Filesystem::createDirectory(dir));
  
  //Check for the existance of a directory that is there.
  // struct stat statbuf;
  ret = false;
  // int rt = lstat(dir.c_str(), &st);
  if(lstat(dir.c_str(), &statbuf) != -1){
    if(S_ISDIR(statbuf.st_mode)){
      ret = true;
    }
  }
  
  EXPECT_TRUE(ret);
  
  //Remove directory to cleanup state.
  
  int rv = std::remove(dir.c_str());

  EXPECT_EQ(rv,0);

}

TEST(FilesystemTest, DirectoryCreate2tier){
  std::string dir = "DirectoryCreate2tier/innerDirectory";

  //Check for the existance of a directory that is not there.
  struct stat statbuf;
  bool ret = false;
  // int rt = lstat(dir.c_str(), &st);
  if(lstat(dir.c_str(), &statbuf) != -1){
    if(S_ISDIR(statbuf.st_mode)){
      ret = true;
    }
  }

  EXPECT_FALSE(ret);

  EXPECT_TRUE(ExpUtils::Filesystem::createDirectory(dir));

  //Check for the existance of a directory that is there.
  // struct stat statbuf;
  ret = false;
  // int rt = lstat(dir.c_str(), &st);
  if(lstat(dir.c_str(), &statbuf) != -1){
    if(S_ISDIR(statbuf.st_mode)){
      ret = true;
    }
  }
  
  EXPECT_TRUE(ret);
  
  //Remove directory to cleanup state.
  
  int rv = std::remove(dir.c_str());
  EXPECT_EQ(rv,0);
  rv = std::remove("DirectoryCreate2tier");
  EXPECT_EQ(rv,0);


}

TEST(FilesystemTest, DirectoryExists){
  std::string dir = "DirectoryExists";
  EXPECT_FALSE(ExpUtils::Filesystem::directoryExists(dir));
  EXPECT_TRUE(ExpUtils::Filesystem::createDirectory(dir));
  EXPECT_TRUE(ExpUtils::Filesystem::directoryExists(dir));
  EXPECT_TRUE(ExpUtils::Filesystem::removeDirectory(dir));
  EXPECT_FALSE(ExpUtils::Filesystem::directoryExists(dir));
}

TEST(FilesystemTest, RemoveDirectory){
  std::string dir = "RemoveDirectory";
  EXPECT_TRUE(ExpUtils::Filesystem::createDirectory(dir));
  //Check for the existance of a directory that is not there.
  EXPECT_TRUE(ExpUtils::Filesystem::directoryExists(dir));
  EXPECT_TRUE(ExpUtils::Filesystem::removeDirectory(dir));
  EXPECT_FALSE(ExpUtils::Filesystem::directoryExists(dir));
}

TEST(FilesystemTest, FileExists){
  std::string file = "tmp.txt";
  EXPECT_FALSE(ExpUtils::Filesystem::fileExists(file));
  std::fstream fs(file,std::ios_base::out);
  fs << "Tmp" << std::endl;
  fs.close();
  EXPECT_TRUE(ExpUtils::Filesystem::fileExists(file));
  unlink(file.c_str());
  EXPECT_FALSE(ExpUtils::Filesystem::fileExists(file));
}

TEST(FilesystemTest, OpenFile){
  std::string file = "tmp.txt";
  EXPECT_FALSE(ExpUtils::Filesystem::fileExists(file));
  std::fstream fs = ExpUtils::Filesystem::openFile(file, std::ios_base::out);
  fs<<"Hello World\nThe world is ending soon\nGoodnight World\n"<<std::flush;
  fs.close();
  EXPECT_TRUE(ExpUtils::Filesystem::fileExists(file));
  ExpUtils::Filesystem::openFile(fs, file, std::ios_base::in);
  // std::string line;
  std::stringstream buf;
  buf << fs.rdbuf();
  // std::getline(fs, line);
  EXPECT_EQ("Hello World\nThe world is ending soon\nGoodnight World\n",buf.str());
  fs.close();
  unlink(file.c_str());
  EXPECT_FALSE(ExpUtils::Filesystem::fileExists(file));
}

TEST(FilesystemTest, OpenNonExistantFile){
  std::string file = "tmp.txt";
  ASSERT_FALSE(ExpUtils::Filesystem::fileExists(file));
  
  std::fstream fs = ExpUtils::Filesystem::openFile(file, std::ios_base::in);
  EXPECT_FALSE(fs.is_open());
  fs = ExpUtils::Filesystem::openFile(file, std::ios_base::out);
  EXPECT_TRUE(fs.is_open());
  fs.close();
  EXPECT_TRUE(ExpUtils::Filesystem::fileExists(file));
  unlink(file.c_str());
  EXPECT_FALSE(ExpUtils::Filesystem::fileExists(file));

}

TEST(FilesystemTest, OpenFileNonExistantDirectory){
  std::string dir = "tmpdir/";
  std::string file = "tmp.txt";
  std::fstream fs = ExpUtils::Filesystem::openFile(dir + file, std::ios_base::out);
  EXPECT_TRUE(fs.is_open());
  fs << "TMP\n" <<std::flush;
  fs.close();
  EXPECT_TRUE(ExpUtils::Filesystem::fileExists(dir + file));
  unlink((dir + file).c_str());
  EXPECT_FALSE(ExpUtils::Filesystem::fileExists(dir + file));
  ExpUtils::Filesystem::removeDirectory(dir);
  EXPECT_FALSE(ExpUtils::Filesystem::directoryExists(dir));
}

TEST(FilesystemTest, PathJoin){

  std::string dir = "tmpdir";
  std::string file = "tmp.txt";

  auto ret = ExpUtils::Filesystem::pathJoin(dir, file);
  EXPECT_EQ(ret, "tmpdir/tmp.txt");

  dir = "tmpdir/";
  file = "tmp.txt";

  ret = ExpUtils::Filesystem::pathJoin(dir, file);
  EXPECT_EQ(ret, "tmpdir/tmp.txt");


}
