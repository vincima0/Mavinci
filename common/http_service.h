
#pragma once
#include <string>
#include <source_location>
#include <boost/beast.hpp>
#include <boost/json/src.hpp>
#include <boost\json\object.hpp>
// namespace beast = boost::beast;     // from <boost/beast.hpp>
// namespace http = beast::http;       // from <boost/beast/http.hpp>
// namespace net = boost::asio;        // from <boost/asio.hpp>
// using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>





class http_service final
{
public:
    static bool send_request(std::string_view host,std::string_view port,
    const boost::beast::http::request<boost::beast::http::string_body>& req,
    boost::beast::http::response<boost::beast::http::string_body>& res)
    {
        //the io_context is required for all I/O
        boost::asio::io_context ioc;
        //these objects perform our I/O
        boost::asio::ip::tcp::resolver resolver(ioc);
        boost::beast::tcp_stream stream(ioc);
        // look up the domain name
        boost::system::error_code ec;
        auto const results = resolver.resolve(host,port,ec);
        if(ec)
        {
            console::log(std::source_location::current(),host,port,ec);
            return false;
        }
        //make the connect on the ip address we get form a lookup
        stream.connect(results,ec);
        if(ec)
        {
            console::log(std::source_location::current(),host,port,ec);
            return false;
        }
        //send the http request to the remote host
        boost::beast::http::write(stream,req,ec);
        if(ec)
        {
            console::log(std::source_location::current(),host,port,ec);
            return false;
        }
        //this buffer id used for reading and must be persisted
        boost::beast::flat_buffer buffer;
        //receive the HTTP response
        boost::beast::http::read(stream,buffer,res,ec);
        if(ec)
        {
            console::log(std::source_location::current(),host,port,ec);
            return false;
        }

        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
        if(ec && ec !=boost::beast::errc::not_connected)
        {
            console::log(std::source_location::current(), host, port, ec);
        }

        return true;




    }



};         