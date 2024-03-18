//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: H//[example_http_client
#include "common_define.h"
#include "console.h"
#include "http_service.h"
#include "temperature_sensor.h"
#include "light_sensor.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/json/src.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>//时间间隔
#include <thread>//线程睡眠
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

// Performs an HTTP GET and prints the response
int main(int argc, char** argv)
{
    try
    {
        const auto& c =std::source_location::current();
        std::cout<<c.column()<<" "<<c.file_name()<<c.function_name()<<"\n";
        auto const host = "127.0.0.1";
        auto const port = "8080";
       
        while(true)
        {
            //获取温度数据
            temperature_sensor temp_sensor;
            float temprature = temp_sensor.get_temperature();

            //发送温度数据
            http_service::send_temperature(host,port,temperature);

            //获取光照数据
            light_sensor light_sensor;
            float light =light_sensor.get_light_intensity();

            //发送光照数据
            http_service::send_light(host,port,light);

            std::this_thread::sleep_for(std::chrono::seconds(5));





        }
         
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//
//
//------------------------------------------------------------------------------

