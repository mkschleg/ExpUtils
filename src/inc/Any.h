#ifndef _EXPUTILS_ANY_H
#define _EXPUTILS_ANY_H

#include "Memory.h"

#include <typeinfo>
// #include <memory>

namespace ExpUtils{
  //! \class ExpUtils::Any
  //! \brief A simple any class mostly for the argparser.
  //! This class is compile time bloaty... Does not work with literal strings yet. Working on a fix.
  class Any{
    template<typename _ValueType>
    friend bool operator==(const _ValueType& lhs, const Any& rhs);
    template<typename _ValueType>
    friend bool operator==(const Any& lhs, const _ValueType& rhs);
  public:
    //I'm not sure the default should be public... Could be useful though.
    Any():value(nullptr) {}

    //Default destructor
    ~Any() = default;

    //Copy Constructor
    Any(const Any& cp): value(cp.value == nullptr ? nullptr : cp.value->clone()) {}

    //Constructor: data is the stored value.
    template<typename _ValueType>
    Any(const _ValueType& data): value(Memory::make_unique<Holder<_ValueType>>(data)) {}

    template<typename _ValueType>
    Any& operator=(const _ValueType& data){
      Any tmp(data);
      return swap(tmp);
    }

    template<typename _ValueType>
    _ValueType retrieve(){
      return Memory::static_unique_pointer_cast<Holder<_ValueType>>(value)->m_storage;
    }

    //Swap is useful
    Any& swap(Any& other) {
      std::swap(value, other.value);
      return *this;
    }
    
    const std::type_info& getType() { return value == nullptr ? typeid(void) : value->getType(); }

  protected:
    class BaseHolder{
    public:
      virtual ~BaseHolder() {}
      virtual const std::type_info& getType() = 0;
      virtual BaseHolder* clone() = 0;
    };
    template<typename _ValueType>
    class Holder: public BaseHolder{
    public:
      _ValueType m_storage;
      Holder(_ValueType data):m_storage(data){}
      const std::type_info& getType() override { return typeid(_ValueType); }
      BaseHolder* clone() override{ return new Holder(m_storage); }
    };

    std::unique_ptr<BaseHolder> value;

  };

  template<typename _ValueType>
  bool operator==(const _ValueType& lhs, const Any& rhs){
    Any::Holder<_ValueType>* h = static_cast<Any::Holder<_ValueType>*>(rhs.value.get());
    return lhs == h->m_storage;
  }
  template<typename _ValueType>
  bool operator==(const Any& lhs, const _ValueType& rhs){
    return rhs == lhs;
  }


} //namespace ExpUtils



#endif
