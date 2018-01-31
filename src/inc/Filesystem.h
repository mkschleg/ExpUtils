#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

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



namespace ExpUtils{
  namespace Filesystem{
    namespace _util{
      std::vector<std::string>* stringExplode(const std::string& source, const std::string& delimiter);
    } //namespace _util
    
    std::pair<std::string,std::string> getDirectoryFilePair(const std::string& source);
    
    bool isDirectory(const std::string& dir);
    bool directoryExists(const std::string& dir);
    bool createDirectory(const std::string& dir);
    bool removeDirectory(const std::string& dir);

    bool fileExists(const std::string& file);
    bool removeFile(const std::string& file);
    bool createFile(const std::string& file);

    // bool forceRemoveDirectory();

    std::fstream openFile(const std::string& file, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void openFile(std::fstream& fs, const std::string& file, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
  }
}


#endif
