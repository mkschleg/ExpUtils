#pragma once

#include "Filesystem.h"

#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>


namespace ExpUtils{

  class _SaveManagerTempObject;
  
  class SaveManager{
    friend class _SaveManagerTempObject;
    friend _SaveManagerTempObject operator<<(SaveManager& saveManager, std::string);
  public:

    SaveManager(){}

    void setDirectory(std::string dir){ directory = dir; }
    // void setDirectory(std::string dir){ directory = dir; }

    template<typename T>
    void operator()(const std::string& file, const T& data){
      save(file, data);
    }

  private:
    int backupFileIdentifier = 0;
    // std::string directory = "";
    std::string directory;
    
    template<typename T>
    void save(const std::string& _file, const T& data){
      std::string file = _file;
      if(file== ""){
        std::cerr<<"SaveManager: Warning filename empty"<<std::endl;
        file = std::to_string(backupFileIdentifier++) + ".dat";
      }
      std::fstream fs = Filesystem::openFile(directory + file, std::fstream::out);

      if(fs.is_open()){
        fs<<data<<"\n"<<std::flush;
        fs.close();
      } else {
        std::cerr<<"Can't open file "<<directory + file<<"\n";
        std::cerr<<"Trying temporary file."<<std::endl;
        std::fstream fs = Filesystem::openFile(std::to_string(backupFileIdentifier++) + ".tmp",std::fstream::out);
        if(fs.is_open()){
          fs<<file<<std::endl;
          fs<<data<<"\n"<<std::flush;
          fs.close();
        } else {
          std::cerr<<"Failed: data lost"<<std::endl;
        }
      }
    }
  };
  
  class _SaveManagerTempObject{
    friend _SaveManagerTempObject operator<<(SaveManager& saveManager, std::string filename);
    SaveManager& m_saveManager;
    std::string m_fileName;
    _SaveManagerTempObject(SaveManager& saveManager, std::string filename):m_saveManager(saveManager), m_fileName(filename){}
    template<typename T>
    void saveData(const T& data){
      m_saveManager.save(m_fileName, data);
    }
  public:
    template<typename T>
    void operator , (const T& data){
      saveData(data);
    }
  };

  
  inline _SaveManagerTempObject operator<<(SaveManager& saveManager, std::string filename){
    // std::cout<<"Here"<<std::endl;
    return _SaveManagerTempObject(saveManager, filename);
  }

}
