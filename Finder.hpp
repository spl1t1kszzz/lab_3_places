// ReSharper disable CppUnusedIncludeDirective
#ifndef FINDER_HPP
#define FINDER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "Config.hpp"
#include "JSON/json.hpp"
#include "JSON/JSON_Formatter.hpp"
#include "API/HTTP_Request.hpp"
#include "API/Weather.hpp"
#include "API/Interesting_place.hpp"


namespace Places {
    using json = nlohmann::json;
    namespace asio = boost::asio;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace ssl = asio::ssl;
    using tcp = asio::ip::tcp;


    class Finder {
        static asio::awaitable<void> start(const std::string&location);

    public:
        static void find_places(const std::string&place);
    };
} // Places

#endif //FINDER_HPP
