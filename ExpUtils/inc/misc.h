#pragma once

#include <vector>
#include <string>


namespace ExpUtils{
namespace _misc{
  template<typename chartype>
  std::vector<std::basic_string<chartype>> stringExplode(const std::basic_string<chartype>& source, std::basic_string<chartype> delimiter){
    int found;
    std::vector<std::basic_string<chartype>> ret;
    found=source.find_first_of(delimiter);
    while(found!=std::string::npos){
      if(found > 0){
        ret->push_back(source.substr(0,found));
      }
      source = source.substr(found+1);
      found = source.find_first_of(delimiter);
    }
    if(source.length()>0) ret->push_back(source);
    return ret;
  }
}
}
