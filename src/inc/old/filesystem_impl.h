#pragma once

#include <string>
#include <vector>

namespace ExpUtils{
  
namespace Filesystem{
  

  class path{
    typedef char value_type;
    typedef std::basic_string<value_type>  string_type;
    static constexpr value_type preferred_separator = '/';

    friend bool operator==(const path& _lhs, const path& _rhs);
    
  public:
    
    //TODO: Char type agnostic constructor
    path() { }
    path(const path& p):m_pathname(p.string()), m_type(p.m_type){
      splitCmpts();
    }
    path(path&& p) noexcept: m_pathname(std::move(p.m_pathname)), m_cmpts(std::move(p.m_cmpts)), m_type(std::move(p.m_type)){
      p.clear();
    }
    path(const string_type& spath): m_pathname(spath){
      splitCmpts();
    }
    path(string_type&& spath): m_pathname(std::move(spath)){
      splitCmpts();
      spath.clear();
    }
    template< class InputIt >
    path( InputIt first, InputIt last ): m_pathname(first,last){
      splitCmpts();
    }
    ~path() = default;

    //Assignment operators.
    //TODO: Char type agnostic assignment.
    path& operator=(const path& _p);
    path& operator=(path&& _p);
    path& operator=(const string_type& _s);
    // template<typename Source>
    // path& operator=(const Source& source);

    //Modifying operators.
    path& operator+=(const path& _p);
    //TODO: Char type agnostic.
    path& operator+=(const string_type& _s);
    
    path& operator/=(const path& _p);
    //TODO: Char type agnostic.
    path& operator/=(const string_type& _s);
    
    //TODO: Char type agnostic.
    path& assign(const string_type& _s);
    template<class InputIt>
    path& assign( InputIt first, InputIt last );

    void  clear() noexcept{
      m_pathname.clear();
    }

    bool empty() const noexcept{
      return m_pathname.empty();
    }

    //Eventually support other string types.
    std::string string() const{return m_pathname;}
    std::string u8string() const{return m_pathname;}
    // void operator
    
    
    //Decomposition
    path root_name() const;
    path root_directory() const;
    path root_path() const;
    path relative_path() const;
    path parent_path() const;
    path filename() const;
    path stem() const;
    path extension() const;
    

    #ifdef _TEST_
    struct cmpt;
    std::vector<cmpt> get_cmpts(){return m_cmpts;}
    unsigned char get_type(){return (unsigned char)m_type;}
    #endif




  private:
    enum class Type : unsigned char {
      multi, rootName, rootDir, filename, dir
    };
    
    path(string_type&& spath, Type _type): m_pathname(std::move(spath)), m_type(_type){}
    

    void splitCmpts();
    
    
    string_type m_pathname;

    #ifndef _TEST_
    struct cmpt;
    #endif
    std::vector<cmpt> m_cmpts;
    Type m_type = Type::multi;

    
  };
  
  path operator+(const path& _lhs, const path& _rhs);
  path operator/(const path& _lhs, const path& _rhs);
  std::ostream& operator<<(std::ostream& os, const path& _rhs);

  bool operator==(const path& _lhs, const path& _rhs);

  // path current_path();
  
  //Assignment operators.
  //TODO: Char type agnostic assignment.
  inline path& path::operator=(const path& _p){
    m_pathname = _p.string();
    splitCmpts();
    return *this;
  }
  inline path& path::operator=(path&& _p){
    m_pathname = std::move(_p.m_pathname);
    _p.clear();
    splitCmpts();
    return *this;
  }
  inline path& path::operator=(const string_type& _s){
    m_pathname = _s;
    splitCmpts();
    return *this;
  }
  // template<typename Source>
  // path& operator=(const Source& source);

  //Modifying operators.
  inline path& path::operator+=(const path& _p){
    m_pathname += _p.m_pathname;
    splitCmpts();
    return *this;
  }
  //TODO: Char type agnostic.
  inline path& path::operator+=(const string_type& _s){
    m_pathname += _s;
    splitCmpts();
    return *this;
  }
  
  inline path& path::operator/=(const path& _p){
    if(m_pathname.back() != preferred_separator) m_pathname += preferred_separator;
    m_pathname = m_pathname + _p.string();
    splitCmpts();
    return *this;
  }
  //TODO: Char type agnostic.
  inline path& path::operator/=(const string_type& _s){
    if(m_pathname.back() != preferred_separator) m_pathname += preferred_separator;
    m_pathname = m_pathname + _s;
    splitCmpts();
    return *this;
  }
  
  //TODO: Char type agnostic.
  inline path& path::assign(const string_type& _s){
    *this = _s;
    splitCmpts();
    return *this;
  }
  template<class InputIt>
  path& path::assign( InputIt first, InputIt last ){
    // splitCmpts();
    return *this;
  }
  
  //Struct for internal components for path;
  struct path::cmpt: path{
    cmpt(string_type _s, Type _t, size_t _pos):path(std::move(_s), _t), pos(_pos){}
    size_t pos;
  };

  

}//namespace Filesystem
}//namespace ExpUtils
