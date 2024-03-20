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
// Example: HTTP server, synchronous
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
#include "common_define.h"
#include "console.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <filesystem>
#include <fstream>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string
path_cat(
    beast::string_view base,
    beast::string_view path)
{
	if (base.empty())
		return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

// Return a response for the given request.
//
// The concrete type of the response message (which depends on the
// request), is type-erased in message_generator.
template <class Body, class Allocator>
http::message_generator
handle_request(
    beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>>&& req)
{
    std::cout<< req.base() <<req.body()<<std::endl;
    // Returns a bad request response
    auto const bad_request =
    [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    // if( req.method() != http::verb::get &&
    //     req.method() != http::verb::head)
    //     return bad_request("Unknown HTTP-method");

    if (req.target() == http_url_router::TEMPERATURE)
    {
        const auto json_str = req.body();
        boost::system::error_code ec;
        const boost::json::value js = boost::json::parse(json_str,ec);
        if(ec)
        {
            std::cout << ec.message() << std::endl;
        }
        else
        {
            if (js.is_object())
            {
                const boost::json::object& obj = js.as_object();
                // 从 JSON 对象中读取各种类型的数据
                if (obj.contains("temperature") && obj.at("temperature").is_number())
                {
                    float temperature = obj.at("temperature").as_double();
                    std::cout << "Temperature: " << temperature << std::endl;
                }

                if (obj.contains("time") && obj.at("time").is_number())
                {
                    std::time_t timestamp = obj.at("time").as_int64();
                    std::cout << "Timestamp: " << std::asctime(std::localtime(&timestamp)) << std::endl;
                }

            // 可以继续读取其他类型的数据，如字符串、布尔值等
            
            } else
            {
                std::cerr << "Received JSON is not an object." << std::endl;
            }
        }
    }


if (req.target() == http_url_router::LIGHT)
{
    const auto json_str = req.body();
    boost::system::error_code ec;
    const boost::json::value js = boost::json::parse(json_str, ec);
    if (ec)
    {
        std::cout << "Error parsing JSON: " << ec.message() << std::endl;
    }
    else
    {
        if (js.is_object())
        {
            const boost::json::object& obj = js.as_object();

            // 从 JSON 对象中读取光照数据
            if (obj.contains("light") && obj.at("light").is_number())
            {
                float light_intensity = obj.at("light").as_double();
                std::cout << "Light Intensity: " << light_intensity << std::endl;
            }

            if (obj.contains("time") && obj.at("time").is_number())
            {
                std::time_t timestamp = obj.at("time").as_int64();
                std::cout << "Timestamp: " << std::asctime(std::localtime(&timestamp)) << std::endl;
            }

            // 可以继续读取其他类型的数据，如字符串、布尔值等
        }
        else
        {
            std::cerr << "Received JSON is not an object." << std::endl;
        }
    }
}

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return bad_request("Illegal request-target");

    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/')
        path.append("index.html");

    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if(ec == beast::errc::no_such_file_or_directory)
        return not_found(req.target());

    // Handle an unknown error
    if(ec)
        return server_error(ec.message());

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if(req.method() == http::verb::head)
    {
        http::response<http::empty_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return res;
    }

    // Respond to GET request
    http::response<http::file_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version())};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, mime_type(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return res;
}

//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Handles an HTTP server connection
void
do_session(
    tcp::socket& socket,
    std::shared_ptr<std::string const> const& doc_root)
{
    beast::error_code ec;

    // This buffer is required to persist across reads
    beast::flat_buffer buffer;

    for(;;)
    {
        // Read a request
        http::request<http::string_body> req;
        http::read(socket, buffer, req, ec);
        if(ec == http::error::end_of_stream)
            break;
        if(ec)
            return fail(ec, "read");

        // Handle request
        http::message_generator msg =
            handle_request(*doc_root, std::move(req));

        // Determine if we should close the connection
        bool keep_alive = msg.keep_alive();

        // Send the response
        beast::write(socket, std::move(msg), ec);

        if(ec)
            return fail(ec, "write");
        if(! keep_alive)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            break;
        }
    }

    // Send a TCP shutdown
    socket.shutdown(tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
}


void create_html()
{
    std::ofstream ofs;
    std::string file_name ="index.html";
    ofs.open(file_name.c_str(),std::ios_base::trunc | std::ios_base::out);
    if (ofs.is_open())
    {
        ofs<< R"(<!DOCTYPE html>
    <html lang="en">
    <head>
    <meta charset="UTF-8">
    <title>Title</title>
    <script src="https://cdn.jsdelivr.net/npm/vue/dist/vue.js"></script>
    </head>
    <body>
        <a href="/server.exe" title="下载文件">server.exe</a>
        <p></p>
        <div id="app">
        <input type="text" v-model="num">
        <table border="1">
        <tr v-for="i in parseInt(num) ">
            <td v-for="j in i">{{j}}*{{i}}={{i*j}}</td>
        </tr>
        </table>
        </div>
        <script>
            var app = new Vue({
                    el: "#app",
                    data: {
                            num:9
                        }
                    });
        </script>
    </body>
    </html>)" << std::endl;
        ofs.close();
    }

}





//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    try
    {
        create_html();
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(std::atoi("8080"));
        const auto& path =std::filesystem::current_path().generic_string();
        auto const doc_root = std::make_shared<std::string>(path);

        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {
            // This will receive the new connection
            tcp::socket socket{ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            std::thread{std::bind(
                &do_session,
                std::move(socket),
                doc_root)}.detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
