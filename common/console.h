#pragma once

#include <mutex>
#include <iostream>
#include <filesystem>
#include <source_location>
#include <boost/date_time.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/system/system_error.hpp>
namespace std
{
    inline ostream& operator << (ostream& ostr,const std::filesystem::path& p)
    {
        
        ostr << p.generic_string();
        return ostr;
    }
    inline ostream& operator << (ostream& ostr,const std::system_error& p)
    {
        
        ostr << "system_error=["<<p.what()<<","<<p.code()<<"]";
        return ostr;
    }
    inline ostream& operator << (ostream& ostr,const boost::system::system_error& p)
    {
       
        ostr << "boost_system_error=["<<p.what()<<","<<p.code()<<"]";
        return ostr;
    }
    inline ostream& operator <<(ostream& ostr,const exception& ec)
    {
        ostr<<ec.what();
        return ostr;
    }
    
    inline ostream& operator <<(ostream& ostr, const error_code& ec)
    {
        ostr<<"error_code=["<<ec.value()<<","<<ec.message()<<"]";
        return ostr;
    }

    inline ostream& operator <<(ostream& ostr,const boost::system::error_code& ec)
    {
        ostr<<"boost_error_code=["<<ec.value()<<","<<ec.message()<<"]";
        return ostr;
    }
}
class console final
{   
    inline static std::mutex  s_cout_mutex;//多线程下cout有竞争问题，加一把互斥锁
    static void Build(std::ostream& log)
    {
        log<<std::endl;
    }
    template<typename T,typename ... Types>
    constexpr static void Build(std::ostream& log,const T& firstArg, const Types& ... args)
    {
        log <<" "<<firstArg;
        Build (log,args...);
    }
private:
    console()= delete;
    ~console()=delete;

    console(const console&)=delete;
    console(console&&);

    console& operator =(const console&) =delete;
    console& operator =(console&&)=delete;
public:
    template<typename ...Types>
    static void log(const Types&...args)
    {
        std::lock_guard<std::mutex> guard{s_cout_mutex};

        static_assert(sizeof...(Types) > 0,"args > 0");
        std::cout<<std::boolalpha;
        if constexpr (sizeof...(Types) > 0)
        {
            const boost::posix_time::ptime& localTime =
                boost::posix_time::microsec_clock::local_time();

            std::cout<<"[";
            std::cout<<boost::posix_time::to_iso_extended_string(localTime);
            std::cout<<" thread:"<<std::setfill(' ')<<std::setw(6);
            std::cout<<std::this_thread::get_id();
            std::cout<<"]";
            Build(std::cout,args...);
        }

    }

};