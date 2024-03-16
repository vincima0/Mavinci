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
// Example: HTTP client, synchronous
//
//------------------------------------------------------------------------------

//[example_http_client
#include "common_define.h"
#include "temperature_sensor.h"
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
       
        auto const host = "127.0.0.1";
        auto const port = "8080";
        auto const target = http_url_router::TEMPERATURE;
        int version =  11;

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        stream.connect(results);

        // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::post, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        
        temperature_sensor sensor;
        boost::json::object data;
        data["temperature"] = sensor.get_temperature();
        data["time"]= std::time(nullptr);
        std::string json_data = boost::json::serialize(data);
        req.body() = json_data;
        req.prepare_payload();

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        std::cout << res << std::endl;
        
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
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes
        // so don't bother reporting it.
        //
        if(ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};

        // If we get here then the connection is closed gracefully
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//]
