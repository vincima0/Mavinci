
#pragma once
#include <string>
#include <source_location>
#include <boost/beast.hpp>
// namespace beast = boost::beast;     // from <boost/beast.hpp>
// namespace http = beast::http;       // from <boost/beast/http.hpp>
// namespace net = boost::asio;        // from <boost/asio.hpp>
// using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>





class http_service final
{
public:
    static bool send_temperature(std::string_view host,std::string_view port,float temperature)
    {
        boost::json::object data;
        data["temperature"] = temperature;
        data["time"] =std::time(nullptr);
        std::string json_data =boost::json::serialize(data);

        http::request<http::string_body> req{http::verb::post,http_url_router::TEMPERATURE,11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.body() = json_data;
        req.prepare_payload();


        http::response<http::string_body> res;
        if(send_request(host,port,req,res))
        {
            console::log(std::source_location::current(),res);
        } 
        else
        {
            return 0;
        }
    }


    static bool send_light(std::string_view host,std::string_view port,float light )
    {
        boost::json::object data;
        data["light"] = light;
        data["time"] =std::time(nullptr);
        std::string json_data =boost::json::serialize(data);

        http::request<http::string_body> req{http::verb::post,http_url_router::TEMPERATURE,11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.body() = json_data;
        req.prepare_payload();


        http::response<http::string_body> res;
        if(send_request(host,port,req,res))
        {
            console::log(std::source_location::current(),res);
        } 
        else
        {
            return 0;
        }
    }

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