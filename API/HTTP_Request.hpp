// ReSharper disable CppUnusedIncludeDirective
#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace Places::API {

    namespace asio = boost::asio;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace ssl = asio::ssl;

    class HTTP_Request {
    public:
        static asio::awaitable<std::string> send_request(const std::string& host, const std::string& port, http::request<http::string_body>& req);
    };
}



#endif //HTTP_REQUEST_HPP
