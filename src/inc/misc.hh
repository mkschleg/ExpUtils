#pragma once

#include <vector>
#include <string>


namespace ExpUtils{
namespace _misc{
  template<typename chartype>
  std::vector<std::basic_string<chartype>> stringExplode(const std::basic_string<chartype>& _source, std::basic_string<chartype> _delimiter){
    size_t found;
    std::string source = _source;
    std::vector<std::basic_string<chartype>> ret;
    found=source.find_first_of(_delimiter);
    while(found!=std::string::npos){
      if(found > 0){
        ret.push_back(source.substr(0,found));
      }
      source = source.substr(found+1);
      found = source.find_first_of(_delimiter);
    }
    if(source.length()>0) ret.push_back(source);
    return ret;
  }
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& _vec){
  for(const auto& d:_vec){
    os<<d<<" ";
  }
  return os;
}

}
