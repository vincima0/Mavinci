#pragma once
class http_service final
{
public:
    static bool send_request(std::string_view host,std::string_view port,
    const boost::beast::http::request<boost::beast::http::string_body>& req,
    boost::beast::http::response<http::string_body>& res)
    {
        //the io_context is required for all I/O
        boost::asio::io_context ioc;
        //these objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);
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
        http::write(stream,req,ec);
        if(ec)
        {
            console::log(std::source_location::current(),host,port,ec);
            return false;
        }
        //this buffer id used for reading and must be persisted
        beast::flat_buffer buffer;
        //receive the HTTP response
        http::read(stream,buffer,res,ec);
        if(ec)
        {
            console::log(std::source_location::current(),host,port,ec);
            return false;
        }

        stream.socket().shutdown(tcp::socked::shutdown_both,ec);
        if(ec && ec !=beast::errc::not_connected)
        {
            console::log(std::source_location::current(), host, port, ec);
        }

        return true;




    }



}          