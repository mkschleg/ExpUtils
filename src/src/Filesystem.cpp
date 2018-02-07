
#include "Filesystem.hh"

// #include "Filesystem/path.cpp"
// #include "Filesystem/directory.cpp"
// #include "Filesystem/fwd.cpp"


#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <iostream>

//FIXME: Depricated
namespace ExpUtils{
  namespace Filesystem{
    namespace _util{
      std::vector<std::string>* stringExplode(const std::string& _source, const std::string& _delimiter){
        int found;
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

    std::pair<std::string,std::string> getDirectoryFilePair(const std::string& source){
      int found;
      std::pair<std::string, std::string> dirfile{"",""};
      found=source.find_last_of('/');
      if(found > 0){
        dirfile.first = source.substr(0,found);
        dirfile.second = source.substr(found+1,source.size());
      } else {
        dirfile.second = source;
      }
      return dirfile;
    }
    
    bool isDirectory(const std::string& dir){
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
    

    bool directoryExists(const std::string& _dir){
      return isDirectory(_dir);
    }

    

    bool createDirectory(const std::string& _dir){
      //Clean given string for directory creation.
      std::string dir = _dir;
      if(dir.back() == '/'){
        // std::cout<<dir<<std::endl;
        dir = dir.substr(0, dir.size()-1);
        // std::cout<<dir<<std::endl;
      }
      //Get path and directory name.
      std::pair<std::string, std::string> dirpair = getDirectoryFilePair(dir);
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
    
    bool removeDirectory(const std::string& dir) {
      // std::rem
      int rv = std::remove(dir.c_str());
      if (rv){
        strerror(errno);
        return false;
      }

      return true;
    }
    
    bool createFile(const std::string& file){
      return false;
    }


    bool fileExists(const std::string& file){
      struct stat buf;
      return (stat(file.c_str(), &buf) == 0);
    }

    bool removeFile(std::string file){
      int rv = std::remove(file.c_str());
      if (rv){
        strerror(errno);
        return false;
      }

      return true;
    }
    
    bool forceRemoveDirectory() {
      //Traverse through directory tree and delete all files and directories.



      return false;
    }

    std::string pathJoin(const std::string& lpath, const std::string& rpath){
      // static constexpr value_type preferred_separator = '/';
      std::string path = lpath;
      if(path.back() != '/') path += '/';
      path = path + rpath;
      return path;
    }

    std::fstream openFile(const std::string& file, std::ios_base::openmode mode){
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

    void openFile(std::fstream& fs, const std::string& file, std::ios_base::openmode mode){
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
