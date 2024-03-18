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
        auto const target = http_url_router::TEMPERATURE_AND_LIGHT;
       

        // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::post, target, 11   };
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        
        temperature_sensor sensor;
        boost::json::object data;
        light_sensor light_sensor;
        data["light_intensity"] =light_sensor.get_light_intensity();
        data["temperature"] = sensor.get_temperature();
        data["time"]= std::time(nullptr);
        std::string json_data = boost::json::serialize(data);
        req.body() = json_data;
        req.prepare_payload();


        
        
        

        http::response<http::string_body> res;
        if(http_service::send_request(host,port,req,res))
        {
            console::log(std::source_location::current(),res);
        } else{
            return 0;
        }

        
        
        std::ofstream ofs;
        std::string file_name = host;
        file_name.append(".txt");
        ofs.open(file_name.c_str(),std::ios_base::trunc | std::ios_base::out);
        if(ofs.is_open())
        {
            ofs<< res << std::endl;
            ofs.close();
        }
        // Gracefully close the socket
        beast::error_code ec;
        
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

