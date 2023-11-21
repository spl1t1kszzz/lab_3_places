#include "Interesting_place.hpp"


namespace Places::API {
    asio::awaitable<json> Interesting_place::find_all_locations(const std::string&location) {
        http::request<http::string_body> req{
            http::verb::get,
            std::string(
                "/api/1/geocode?q=" + location + "&locale=" + Config::LOCALE + "&key=" + Config::LOCATIONS_API_KEY),
            Config::HTTP_VERSION
        };
        auto response = co_await HTTP_Request::send_request("graphhopper.com", "443", req);
        auto result = json::parse(response);
        co_return result["hits"].empty() ? nullptr : result["hits"];
    }

    asio::awaitable<std::string> Interesting_place::get_interesting_places(const json&location) {
        const auto lat = std::to_string(location["point"]["lat"].get<double>());
        const auto lon = std::to_string(location["point"]["lng"].get<double>());
        http::request<http::string_body> request{
            http::verb::get,
            "/0.1/en/places/radius?radius=" + std::to_string(Config::RADIUS) + "&lon=" + lon + "&lat=" + lat + "&limit="
            +
            std::to_string(Config::LIMIT_OF_INT_PLACES) + "&apikey=" +
            Config::INTERESTING_PLACES_API_KEY,
            Config::HTTP_VERSION
        };
        auto response = co_await HTTP_Request::send_request("api.opentripmap.com", "443", request);
        std::stringstream ss;
        for (auto features = json::parse(response)["features"]; const auto& feature: features) {
            ss << JSON_Formatter::format_print(Config::INT_PLACE_KEYS, feature) +
                JSON_Formatter::format_print(Config::INT_PLACE_KEYS, feature["properties"]) << std::endl;
            ss << co_await get_interesting_place_info(feature["id"].get<std::string>()) <<
                    std::endl << std::endl;
        }
        co_return ss.str();
    }

    asio::awaitable<std::string> Interesting_place::get_interesting_place_info(const std::string&id) {
        http::request<http::string_body> request{
            http::verb::get, "/0.1/en/places/xid/" + id + "?apikey=" + Config::INTERESTING_PLACES_API_KEY,
            Config::HTTP_VERSION
        };
        auto response = co_await HTTP_Request::send_request("api.opentripmap.com", "443", request);
        if (json::parse(response)["wikipedia_extracts"]["text"].is_null()) {
            co_return std::string("no desc");
        }
        co_return json::parse(response)["wikipedia_extracts"]["text"].get<std::string>();
    }
}
