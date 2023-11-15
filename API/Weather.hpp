// ReSharper disable CppUnusedIncludeDirective
#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "HTTP_Request.hpp"
#include "../Config.hpp"
#include "../JSON/json.hpp"
#include "../JSON/JSON_Formatter.hpp"

namespace Places::API {
    using json = nlohmann::json;
    namespace asio = boost::asio;
    namespace beast = boost::beast;
    namespace http = beast::http;

    class Weather {
        constexpr static auto WEATHER_API_KEY = "25e4b3636234c63796164d8790d52b01";

    public:
        static asio::awaitable<std::string> get_weather(const json&location);
    };
}


#endif //WEATHER_HPP
