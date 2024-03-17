#pragma once

#include <mutex>
#include <iostream>
#include <boost/date_time.hpp>
class console final
{   
    inline static std::mutex  s_cout_mutex;//多线程下cout有竞争问题，加一把互斥锁
    static void Build(std::ostream& log)
    {
        log<<std::endl;
    }
    template<typename T,typename ...Types>
    constexpr static void Build(std::ostream& log,const T& firstArg, const Types& ... args)
    {
        log<<" "<<firstArg;
    }
private:
    console()= delete;
    ~console()=delete;

    console(const console&)=delete;
    console(console&&);

    console& operator =(const console&) =delete;
    console& operator =(console&&)=delete;
public:
    static void log(...)
    {

    }

};