#ifndef _ARGPARSER_H
#define _ARGPARSER_H

#include "Parser.h"
#include "Any.h"

#include <vector>
#include <string>
#include <map>
#include <typeinfo>

namespace ExpUtils{

  //! \class ArgParser
  //! \brief This is used when a tupple parser isn't enough when parsing command line arguments. This is developed based on the Python3 ArgParser utility
  // I'm pretty sure we might need a lot of constexpr here to be able to implement this. This should be c++14+ only feature. I'm goint to try to have 2
  //   versions one for pre-c++14 and one for post-c++14...

class ArgParser{
public:

  struct Args;
  struct ArgumentFormat;
  struct Help;
  struct Argument;



  ArgParser(){}
  ~ArgParser(){}

  template<typename T>
  void addArgument(std::string name, std::string shortname, size_t size){
    ArgumentFormat form{name,typeid(T),size};

  }


  //Add Arguments
  void addArgument(std::string name, int size){
    // m_nameToArgumentMap[name] = Argument(name);
  }
  void addArgument(std::string shortName, std::string name, int size);
  Args parse(size_t argc, const char* argv[]){
    return Args{};
  }
  Args parse(size_t argc, const char* argv[], Help& help){
    return Args{};
  }

  Args parse(std::vector<std::string> argv){
    return Args{};
  }

  struct ArgumentFormat{
    
    enum Action{
      store,
      store_true,
      store_false,
      append,
      version
    };

    ArgumentFormat(std::string _name, std::string _shortName, std::string _desc, const std::type_info& _type, size_t _size):
      name(_name),shortName(_shortName),desc(_desc),type(_type),size(_size){}
    ArgumentFormat(std::string _name, const std::type_info& _type, size_t _size):ArgumentFormat(_name, "", "", _type, _size){}
    ArgumentFormat(std::string _name, const std::type_info& _type):ArgumentFormat(_name, "", "", _type, 1){}
    
    ArgumentFormat(std::string _name, std::string _desc, const std::type_info& _type, size_t _size);

    std::string name;
    std::string shortName;
    std::string desc;
    const std::type_info& type;
    size_t size;
    Action action;

    // std::function<void (void)> action = [](){};s

    // enum Action{
    //   store,
    //   store_true,
    //   store_false,
    //   append,
    //   version
    // };

  };

  struct Argument{

    std::string m_name;
    std::string m_shortName;

    Any value;
    const std::type_info& type;

    Argument(std::string name):m_name(name),type(typeid(void)){}

    template <typename _ValueType>
    _ValueType retrieve(const std::string& key){
      return operator[](key).retrieve<_ValueType>();
    }
  };

  struct Args{
    std::map<std::string,Argument> m_nameToArgumentMap;


    Args() = default;

    Any operator[](const std::string& key){
      return Any{};
    }
    
    template <typename _ValueType>
    _ValueType retrieve(const std::string& key){
      return operator[](key).retrieve<_ValueType>();
    }

  };

  struct Help{
    //List of argument names, shortnames, optional, types, and descriptions.
  };

private:
  std::vector<ArgumentFormat> orderedArguments;
  std::vector<ArgumentFormat> optionalArguments;

  std::map<std::string, ArgumentFormat> m_nameToFormatMap;
  Help help;


};

inline std::ostream& operator<<(std::ostream& os, const ArgParser::Help& help){
  return os;
}


} //namespace ExpUtils




#endif
