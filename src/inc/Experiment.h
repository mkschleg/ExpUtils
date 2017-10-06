#pragma once

#include <type_traits>

// namespace ExpUtils{
//   
//   // Find out whether U has `void f()` member
// namespace {
//   // template <typename U, typename ...Args>
//   // struct has_member_init {
//   //   typedef char yes;
//   //   struct no { char _[2]; };
//   //   template<typename T, decltype(std::result_of<decltype(&T::init)(Args...)>::type) (T::*)(Args...) = &T::init>
//   //   static yes impl( T* );
//   //   static no  impl(...);
//   // 
//   //   enum { value = sizeof( impl( static_cast<U*>(0) ) ) == sizeof(yes) };
//   // };
//   // template<class Class, typename Type = int>
//   // struct HasMember_init
//   // {
//   //   typedef char (&yes)[2];
//   //   template<unsigned long> struct exists;
//   //   template<typename V> static yes Check (exists<sizeof(static_cast<Type>(&V::init))>*);
//   //   template<typename> static char Check (...);
//   //   static const bool value = (sizeof(Check<Class>(0)) == sizeof(yes));
//   // };
//   // 
//   
//   template <typename T>
//   class has_init
//   {
//     typedef char one;
//     typedef long two;
//     
//     template <typename C> static one test(  char[sizeof(&C::helloworld)]  ) ;
//     template <typename C> static two test(...);
//     
//   public:
//     enum { value = sizeof(test<T>(0)) == sizeof(char) };
//   };
// 
//   template <typename U, typename ...Args>
//   struct has_member_reset {
//     typedef char yes;
//     struct no { char _[2]; };
//     template<typename T, std::result_of<decltype(&T::reset)(Args...)> (T::*)(Args...) = &T::reset>
//     static yes impl( T* );
//     static no  impl(...);
// 
//     enum { value = sizeof( impl( static_cast<U*>(0) ) ) == sizeof(yes) };
//   };
//   
//   template <typename U, typename ...Args>
//   struct has_member_execute {
//     typedef char yes;
//     struct no { char _[2]; };
//     template<typename T, std::result_of<decltype(&T::execute)(Args...)> (T::*)(Args...) = &T::execute>
//     static yes impl( T* );
//     static no  impl(...);
// 
//     enum { value = sizeof( impl( static_cast<U*>(0) ) ) == sizeof(yes) };
//   };
// 
// 
// } //namespace empty
// 
//   //A useful interface for experiments.
//   template <typename derived>
//   class Experiment{
//   public:
//     template<typename ...Args>
//     auto init(Args... args){
//       static_assert(has_init<derived>::value,"Class does not have init function");
//       return static_cast<derived*>(this)->init(std::forward<Args>(args)...);
//     }
//     // virtual void init(...) = 0;
// 
//     template<typename ...Args>
//     auto reset(Args... args){
//       static_assert(has_member_reset<derived,Args...>::value,"Class does not have reset function");
//       return static_cast<derived*>(this)->reset(std::forward<Args>(args)...);
//     }
// 
//     template<typename ...Args>
//     auto operator()(Args... args){
//       static_assert(has_member_execute<derived,Args...>::value,"Class does not have init function");
//       return static_cast<derived*>(this)->execure(std::forward<Args>(args)...);
//     }
// 
// 
//   };
// } //namespace ExpUtils
