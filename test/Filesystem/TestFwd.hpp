#define _TEST_ 1

#include "gtest/gtest.h"
#include "Filesystem/directory.h"
// #include "Filesystem/path.h"
#include <dirent.h>

namespace fs = ExpUtils::Filesystem;


TEST(FileStatus, perms){
  EXPECT_EQ(fs::_smode_to_file_perms(S_IRWXU | S_IFCHR), fs::perms::owner_all);
  EXPECT_EQ(fs::_smode_to_file_perms(S_IRUSR | S_IFBLK), fs::perms::owner_read);
  // EXPECT_EQ(fs::_smode_to_file_perms(S_IRWXU | S_IRWXO | S_IFCHR), fs::perms::owner_all | fs::perms::others_all);
}

TEST(FileStatus, file_type){
  EXPECT_EQ(fs::_smode_to_file_type(S_IRWXU | S_IFCHR), fs::file_type::character);
  EXPECT_EQ(fs::_smode_to_file_type(S_IRUSR | S_IFBLK), fs::file_type::block);
}

TEST(FileStatus, status){
  
}
