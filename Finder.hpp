// ReSharper disable CppUnusedIncludeDirective
#ifndef FINDER_HPP
#define FINDER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl.hpp>
#include "json.hpp"


namespace Places {
    using json = nlohmann::json;
    namespace asio = boost::asio;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace ssl = asio::ssl;
    using tcp = asio::ip::tcp;


    class Finder {
        constexpr static auto PLACES_API_KEY = "8f06ab6d-727d-453d-a63c-db843b782335";

        constexpr static auto WEATHER_API_KEY = "25e4b3636234c63796164d8790d52b01";

        constexpr static auto LOCALE = "en";

        constexpr static auto HTTP_VERSION = 11;

        static asio::awaitable<json> find_all_places(const std::string&place);

        static asio::awaitable<void> start(const std::string& place);

        static asio::awaitable<void> get_weather(const json& location);

        static std::string format_print(const std::map<std::string, bool>& keys, const json& object);

        static asio::awaitable<std::string> send_request(const std::string& host, const std::string& port, const http::request<http::string_body>& req);

    public:
        static void find_places(const std::string&place);
    };
} // Places

#endif //FINDER_HPP
