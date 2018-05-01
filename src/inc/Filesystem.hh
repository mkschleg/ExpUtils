#pragma once

// #include "filesystem_impl.h"

// #include "Filesystem/fwd.h"
// #include "Filesystem/path.h"
// #include "Filesystem/error.h"
// #include "Filesystem/directory.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include <fstream>
#include <vector>
#include <string>

#include <iostream>

//FIXME: Depricated
namespace ExpUtils{
  namespace Filesystem{
    // #ifndef __clang
    // char* perror(int errnum) {return strerror(errnum);}
    // #endif
    namespace _util{
      inline std::vector<std::string>* stringExplode(const std::string& _source, const std::string& _delimiter){
        size_t found = 0;
        std::string source = _source;
        std::vector<std::string>* results = new std::vector<std::string>;
        found=source.find_first_of(_delimiter);
        while(found!=std::string::npos){
          if(found > 0){
            results->push_back(source.substr(0,found));
          }
          source = source.substr(found+1);
          found = source.find_first_of(_delimiter);
        }
        if(source.length()>0) results->push_back(source);
        return results;
      }
    } // namespace _util

    inline std::pair<std::string,std::string> getDirectoryFilePair(const std::string& source){
      size_t found = 0;
      std::pair<std::string, std::string> dirfile{"",""};
      found=source.find_last_of("/");
      //std::cout<<found<<std::endl;
      if(found > 0 && found != std::string::npos){
        dirfile.first = source.substr(0,found);
        dirfile.second = source.substr(found+1,source.size());
      } else {
        dirfile.second = source;
      }
      return dirfile;
    }
    
    inline bool isDirectory(const std::string& dir){
      struct stat statbuf;
      bool ret = false;
      // int rt = lstat(dir.c_str(), &st);
      if(lstat(dir.c_str(), &statbuf) != -1){
        if(S_ISDIR(statbuf.st_mode)){
          ret = true;
        }
      }
      return ret;
    }
    

    inline bool directoryExists(const std::string& _dir){
      return isDirectory(_dir);
    }

    

    inline bool createDirectory(const std::string& _dir){
      //Clean given string for directory creation.
      std::string dir = _dir;
      std::cout<<_dir<<std::endl;
      if(dir.back() == '/'){
        // std::cout<<dir<<std::endl;
        dir = dir.substr(0, dir.size()-1);
        // std::cout<<dir<<std::endl;
      }
      //Get path and directory name.
      std::pair<std::string, std::string> dirpair = getDirectoryFilePair(dir);
      //std::cout<<dir<<" "<<dirpair.first<<" "<<dirpair.second<<std::endl;
      //If the path is relative and simple
      if(dirpair.first == ""){
        if(directoryExists(dir)){
          // std::cerr<<"Directory Exists: "<<dir<<std::endl;
          return true;
        } else {
          if(mkdir(dir.c_str(),0777) == 0){
            // std::cerr<<"Directory Created"<<std::endl;
            return true;
          } else {
            // std::cerr<<"error"<<std::endl;
            return false;
          }
        }
      } else {
        //Else if it is complex.
        createDirectory(dirpair.first);
        if(directoryExists(dir)){
          // std::cerr<<"Directory Exists: "<<dir<<std::endl;
          return true;
        } else {
          if(mkdir(dir.c_str(),0777) == 0){
            // std::cerr<<"Directory Created"<<std::endl;
            return true;
          } else {
            // std::cerr<<"error"<<std::endl;
            return false;
          }
        }
      }
      return false;
    }
    
    inline bool removeDirectory(const std::string& dir) {
      // std::rem
      int rv = std::remove(dir.c_str());
      if (rv){
        strerror(errno);
        return false;
      }

      return true;
    }
    
    inline bool createFile(const std::string& file){
      std::cout<<"File not created: "<<file<<std::endl;
      return false;
    }


    inline bool fileExists(const std::string& file){
      struct stat buf;
      return (stat(file.c_str(), &buf) == 0);
    }

    inline bool removeFile(std::string file){
      int rv = std::remove(file.c_str());
      if (rv){
        strerror(errno);
        return false;
      }

      return true;
    }
    
    inline bool forceRemoveDirectory() {
      //Traverse through directory tree and delete all files and directories.
      return false;
    }

    inline std::string pathJoin(const std::string& lpath, const std::string& rpath){
      // static constexpr value_type preferred_separator = '/';
      std::string path = lpath;
      if(path.back() != '/') path += '/';
      path = path + rpath;
      return path;
    }

    inline std::fstream openFile(const std::string& file, std::ios_base::openmode mode){
      std::pair<std::string, std::string> dirpair = getDirectoryFilePair(file);
      if(dirpair.first != ""){
        createDirectory(dirpair.first);
      }
      std::fstream fs(file, mode);
      if(fs.is_open() == false){
        std::cerr << "File not created: " << file << "\n";
        std::cerr << "Error: " << strerror(errno) << "\n" << std::flush;
      }
      // fs<<"openfile"<<std::endl;
      return fs;
    }

    inline void openFile(std::fstream& fs, const std::string& file, std::ios_base::openmode mode){
      fs.close();
      std::pair<std::string, std::string> dirpair = getDirectoryFilePair(file);
      if(dirpair.first != ""){
        createDirectory(dirpair.first);
      }
      // std::cout<<file<<std::endl;
      fs.open(file, mode);
      if(fs.is_open() == false){
        std::cerr << "File not created: " << file << "\n";
        std::cerr << "Error: " << strerror(errno) << "\n" << std::flush;
      }
    }
  }
}

// namespace ExpUtils{
//   namespace Filesystem{
//     namespace _util{
//       std::vector<std::string>* stringExplode(const std::string& source, const std::string& delimiter);
//     } //namespace _util

//     std::pair<std::string,std::string> getDirectoryFilePair(const std::string& source);

//     bool isDirectory(const std::string& dir);
//     bool directoryExists(const std::string& dir);
//     bool createDirectory(const std::string& dir);
//     bool removeDirectory(const std::string& dir);

//     bool fileExists(const std::string& file);
//     bool removeFile(const std::string& file);
//     bool createFile(const std::string& file);

//     std::string pathJoin(const std::string& lpath, const std::string& rpath);

//     // bool forceRemoveDirectory();

//     std::fstream openFile(const std::string& file, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//     void openFile(std::fstream& fs, const std::string& file, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//   }
// }



