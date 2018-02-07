#pragma once

#include "misc.hh"

#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <exception>
#include <iostream>


namespace ExpUtils{
  

  template<typename T>
  bool convert(const std::string& s, T& dat){
    std::stringstream iss(s);
    // std::cout<<iss.fail()<<" "<<iss.tellg()<<" "<<std::endl;
    iss >> dat;
    // std::cout<<iss.fail()<<" "<<iss.bad()<<" "<<iss.tellg()<<std::endl;
    return !iss.fail();
  }

  template<typename T>
  bool convert(const std::string& s, std::vector<T>& datVec, std::string delim = " "){
    std::vector<std::string> strVec = _misc::stringExplode(s, delim);
    datVec.resize(strVec.size());
    bool ret = true;
    for(int i = 0; i < datVec.size(); i++){
      ret = convert(strVec[i],datVec[i]);
    }
    return ret;
  }

  template<typename T, std::size_t N>
  bool convert(const std::string& s, std::array<T,N>& datVec, std::string delim = " "){
    std::vector<std::string> strVec = _misc::stringExplode(s, delim);
    // datVec.resize(strVec.size());
    if(strVec.size() != N) return false;
    bool ret = true;
    for(int i = 0; i < datVec.size(); i++){
      ret = convert(strVec[i],datVec[i]);
    }
    return ret;
  }


  namespace _misc
  {
    #if __cplusplus > 201103L

    // template<size_t _Np>
    using std::index_sequence;
    using std::make_index_sequence;

    #else
    template <size_t... Ints>
    struct index_sequence
    {
      using type = index_sequence;
      using value_type = size_t;
      static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
    };

    // --------------------------------------------------------------

    template <class Sequence1, class Sequence2>
    struct _merge_and_renumber;

    template <size_t... I1, size_t... I2>
    struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1)+I2)...>
    { };

    // --------------------------------------------------------------

    template <size_t N>
    struct make_index_sequence
    : _merge_and_renumber<typename make_index_sequence<N/2>::type,
    typename make_index_sequence<N - N/2>::type>
    { };

    template<> struct make_index_sequence<0> : index_sequence<> { };
    template<> struct make_index_sequence<1> : index_sequence<0> { };
    #endif

    template<typename T>
    bool ParseList(const std::vector<std::string>& _slist, int i, T& dat){
      return convert(_slist[i], dat);
    }

    template<typename First, typename ...Params>
    bool ParseList(const std::vector<std::string>& _slist, int i, First& firstparam, Params&... params){
      bool ret = convert(_slist[i], firstparam);
      if(i+1 != _slist.size()){
        return ret && ParseList(_slist, i+1, params...);
      } else {
        return false;
      }
      return ret;
    }

    template<typename ...Types, size_t... IS>
    bool ParseList(const std::vector<std::string>& _slist, std::tuple<Types...>& t, index_sequence<IS...>){
      return ParseList(_slist, 0, std::get<IS>(t)...);
    }

  } // namespace _misc


  template<typename ...Params>
  bool ParseList(const std::vector<std::string>& _slist, Params&... params){
    return _misc::ParseList(_slist, 0, params...);
  }
  
  template<typename ...Types>
  bool ParseList(const std::vector<std::string>& _slist, std::tuple<Types...>& t){
    return _misc::ParseList(_slist, t, _misc::make_index_sequence<std::tuple_size<std::tuple<Types...>>{}>{});
  }

  template <typename ...Types>
  std::tuple<Types...> ParseList(const std::vector<std::string>& _slist){
    std::tuple<Types...> ret;
    if(!_misc::ParseList(_slist, ret, _misc::make_index_sequence<std::tuple_size<std::tuple<Types...>>{}>{})){
      throw std::runtime_error("ParseList: unable to Parse Based on Types.");
    }
    return ret;
  }

  template <typename ...Types>
  bool ParseString(const std::string& str, std::tuple<Types...>& t, std::string delim = " "){
    std::vector<std::string> _slist = _misc::stringExplode(str, delim);
    return _misc::ParseList(_slist, 0, t, _misc::make_index_sequence<std::tuple_size<std::tuple<Types...>>{}>{});
  }


  template <typename ...Types>
  std::tuple<Types...> ParseString(const std::string& str, std::string delim = " "){
    std::tuple<Types...> ret;
    if(!ParseString(str, ret, delim)){
      throw std::runtime_error("ParseString: unable to Parse Based on Types.");
    }
  }
  
  template <typename T>
  std::vector<T> ParseToVector(const std::string& str, std::string delim = " "){
    // std::vector<T> ret;
    std::vector<std::string> _slist = _misc::stringExplode(str, delim);
    std::vector<T> ret(_slist.size());
    // ret.resize(size_type __sz)
    for (int i = 0; i < _slist.size(); i++){
      convert(_slist[i], ret[i]);
    }
  }

} // namespace ExpUtils
