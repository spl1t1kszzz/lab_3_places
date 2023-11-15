#include "Weather.hpp"


namespace Places::API {
    asio::awaitable<std::string> Weather::get_weather(const json&location) {
        const auto lat = location["point"]["lat"].get<double>();
        const auto lon = location["point"]["lng"].get<double>();
        http::request<http::string_body> request{
            http::verb::get,
            std::string(
                "/data/2.5/weather?lat=" + std::to_string(lat) + "&lon=" + std::to_string(lon) + "&units=metric&appid="
                + WEATHER_API_KEY),
            Config::HTTP_VERSION
        };
        auto response = co_await HTTP_Request::send_request("api.openweathermap.org", "443", request);
        co_return JSON_Formatter::format_print(Config::WEATHER_KEYS, json::parse(response)["main"]);
    }
}
