 #pragma once


#ifndef DEBUGLEVEL
#define DEBUGLEVEL 0
#endif

#include <iostream>
#include <sstream>

namespace ExpUtils{namespace Log{

class Stream: public std::ostringstream{
public:
    static Stream& getInstance(){
        static Stream instance;
        return instance;
    }
    Stream(Stream&) = delete;
    void operator=(Stream&) = delete;
private:
    Stream() = default;
};

// inline void DEBUG(const std::string&);




}} // namespace ExpUtils::Log
