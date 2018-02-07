#ifndef MAKE_UNIQUE_H_INCLUDED
#define MAKE_UNIQUE_H_INCLUDED

#include <memory>
#include <utility>

namespace ExpUtils{
  namespace Memory {
    #if __cplusplus > 201103L // After c++11 (as of now c++14 and c++17)
    
    using std::make_unique ;

    #else // C++11
    
    template < typename T, typename... CONSTRUCTOR_ARGS >
    std::unique_ptr<T> make_unique( CONSTRUCTOR_ARGS&&... constructor_args )
    { return std::unique_ptr<T>( new T( std::forward<CONSTRUCTOR_ARGS>(constructor_args)... ) ); }

    #endif // __cplusplus == 201402L
    
    using std::make_shared;

    template< typename V, typename T>
    const V* static_unique_pointer_cast(std::unique_ptr<T>& up) {
      return static_cast<V*>(up.get());
    }
    
    template< typename V, typename T>
    const V* dynamic_unique_pointer_cast(std::unique_ptr<T>& up) {
      return dynamic_cast<V*>(up.get());
    }

  }
}

#endif // MAKE_UNIQUE_H_INCLUDED 
