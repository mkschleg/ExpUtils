#define _TEST_ 1

#include "gtest/gtest.h"
#include "Filesystem/directory.h"
// #include "Filesystem/path.h"
#include <dirent.h>
namespace fs = ExpUtils::Filesystem;

TEST(DirectoryTest, misc){
  fs::path m_path("dir/");
  DIR* dir = opendir(m_path.string().c_str());
  if(dir != nullptr){
    //Lets look through the contents.
    struct dirent* d;
    while((d = readdir(dir)) != nullptr){
      if (d != nullptr){
        std::cout<<d->d_name<<std::endl;
      }
    }
    closedir(dir);

    // d->

  } else {
    //Error! What should I do?? probably nothing...
    // throw strerror(errno);
  }
}



TEST(DirectoryTest, directory_iterator){
  fs::directory_iterator di(fs::path("dir/"));
  // std::cout<<"Hello"<<std::endl;
  // di++;
  // di.operator++();
  // di++;

  std::cout<<(*di).path()<<std::endl;
  ++di;
  std::cout<<(*di).path()<<std::endl;
}
// 
// 
TEST(DirectoryTest, recursive_directory_iterator){
  fs::directory_iterator di(fs::path("dir/"));
  // std::cout<<"Hello"<<std::endl;
  // for()
  std::cout<<(*di).path()<<std::endl;
  ++di;
  std::cout<<(*di).path()<<std::endl;
  ++di;
  std::cout<<(*di).path()<<std::endl;
}
