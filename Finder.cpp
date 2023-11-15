#include "Finder.hpp"


namespace Places {

    void Finder::find_places(const std::string&place) {
        asio::io_context io_context;
        co_spawn(io_context, [&place] { return start(place); }, asio::detached);
        io_context.run();
    }

    asio::awaitable<json> Finder::find_all_places(const std::string&place) {
        http::request<http::string_body> req{
            http::verb::get,
            std::string("/api/1/geocode?q=" + place + "&locale=" + Config::LOCALE + "&key=" + PLACES_API_KEY),
            Config::HTTP_VERSION
        };
        auto response = co_await API::HTTP_Request::send_request("graphhopper.com", "443", req);
        co_return response.empty() ? nullptr : json::parse(response)["hits"];
    }


    asio::awaitable<void> Finder::start(const std::string&place) {
        const auto json = co_await find_all_places(place);
        if (json == nullptr) {
            co_return;
        }
        // 1st string - key in json, 2nd key - key to display
        const std::map<std::tuple<std::string, std::string, bool>, bool> keys{
            {{"postcode", "Postcode", false}, true}, {{"country", "Country", false}, true},
            {{"state", "State", false}, true},
            {{"name", "name", false}, true}
        };
        for (int i = 0; i < json.size(); ++i) {
            std::cout << "Location " << i + 1 << ": " << JSON_Formatter::format_print(keys, json[i]) << std::endl <<
                    std::endl;
        }
        std::cout << "Choose location: ";
        int chosen_location;
        std::cin >> chosen_location;
        std::stringstream result;
        result << std::string("Weather in ") + json[chosen_location - 1]["name"].get<std::string>() + ": " + co_await
                API::Weather::get_weather(json[chosen_location - 1]);
        result << std::endl << co_await get_interesting_places(json[chosen_location - 1]);
        std::cout << result.str() << std::flush;
    }


    asio::awaitable<std::string> Finder::get_interesting_places(const json&location) {
        const auto lat = std::to_string(location["point"]["lat"].get<double>());
        const auto lon = std::to_string(location["point"]["lng"].get<double>());
        http::request<http::string_body> request{
            http::verb::get,
            "/0.1/en/places/radius?radius=" + std::to_string(Config::RADIUS) + "&lon=" + lon + "&lat=" + lat + "&limit="
            +
            std::to_string(Config::LIMIT_OF_INT_PLACES) + "&apikey=" +
            INTERESTING_PLACES_API_KEY,
            Config::HTTP_VERSION
        };
        auto response = co_await API::HTTP_Request::send_request("api.opentripmap.com", "443", request);
        const std::map<std::tuple<std::string, std::string, bool>, bool> keys{
            {{"id", "ID", false}, false}, {{"name", "Name", true}, true}, {{"address", "Address", true}, true}
        };
        std::stringstream ss;
        for (int i = 0; i < json::parse(response)["features"].size(); ++i) {
            ss << JSON_Formatter::format_print(keys, json::parse(response)["features"][i]) +
                    JSON_Formatter::format_print(
                        keys, json::parse(response)["features"][i]["properties"]) << std::endl;
            ss << co_await get_interesting_place_info(json::parse(response)["features"][i]["id"].get<std::string>()) <<
                    std::endl << std::endl;
        }
        co_return ss.str();
    }


    asio::awaitable<std::string> Finder::get_interesting_place_info(const std::string&id) {
        http::request<http::string_body> request{
            http::verb::get, "/0.1/en/places/xid/" + id + "?apikey=" + INTERESTING_PLACES_API_KEY,
            Config::HTTP_VERSION
        };
        auto response = co_await API::HTTP_Request::send_request("api.opentripmap.com", "443", request);
        if (json::parse(response)["wikipedia_extracts"]["text"].is_null()) {
            co_return std::string("no desc");
        }
        co_return json::parse(response)["wikipedia_extracts"]["text"].get<std::string>();
    }
} // Places
