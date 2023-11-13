#include "Finder.hpp"

namespace Places {
    asio::awaitable<json> Finder::find_all_places(const std::string&place) {
        http::request<http::string_body> req{
            http::verb::get, std::string("/api/1/geocode?q=" + place + "&locale=" + LOCALE + "&key=" + PLACES_API_KEY),
            HTTP_VERSION
        };
        req.set(http::field::host, "graphhopper.com");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        auto response = co_await send_request("graphhopper.com", "443", req);
        co_return response.empty() ? nullptr : json::parse(response)["hits"];
    }


    void Finder::find_places(const std::string&place) {
        asio::io_context io_context;
        co_spawn(io_context, [&place] { return start(place); }, asio::detached);
        io_context.run();
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
            std::cout << "Location " << i + 1 << ": " << format_print(keys, json[i]) << std::endl << std::endl;
        }
        std::cout << "Choose location: ";
        int chosen_location;
        std::cin >> chosen_location;
        std::stringstream result;
        // auto a = co_await co_spawn(co_await asio::this_coro::executor, [&json, chosen_location] { return get_weather(json[chosen_location - 1]);}, asio::use_awaitable);
        result << std::string("Weather in ") + json[chosen_location - 1]["name"].get<std::string>() + ": " + co_await
                get_weather(json[chosen_location - 1]);
        // result << "Weather in " +  co_await get_weather(json[chosen_location - 1]);
        result << std::endl << co_await get_interesting_places(json[chosen_location - 1]);
        std::cout << result.str();
    }

    asio::awaitable<std::string> Finder::get_weather(const json&location) {
        const auto lat = location["point"]["lat"].get<double>();
        const auto lon = location["point"]["lng"].get<double>();
        http::request<http::string_body> request{
            http::verb::get,
            std::string(
                "/data/2.5/weather?lat=" + std::to_string(lat) + "&lon=" + std::to_string(lon) + "&units=metric&appid="
                + WEATHER_API_KEY),
            HTTP_VERSION
        };
        request.set(http::field::host, "api.openweathermap.org");
        request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        auto response = co_await send_request("api.openweathermap.org", "443", request);

        const std::map<std::tuple<std::string, std::string, bool>, bool> keys{
            {{"temp", "Temerature (C)", true}, true}, {{"feels_like", "Feels like (C)", true}, true},
            {{"humidity", "Humidity (%)", true}, true}, {{"pressure", "Pressure (hPa)", true}, true}
        };
        co_return format_print(keys, json::parse(response)["main"]);
    }

    asio::awaitable<std::string> Finder::get_interesting_places(const json&location) {
        const auto lat = std::to_string(location["point"]["lat"].get<double>());
        const auto lon = std::to_string(location["point"]["lng"].get<double>());
        http::request<http::string_body> request{
            http::verb::get,
            "/0.1/en/places/radius?radius=" + std::to_string(RADIUS) + "&lon=" + lon + "&lat=" + lat + "&limit=" +
            std::to_string(LIMIT_OF_INT_PLACES) + "&apikey=" +
            INTERESTING_PLACES_API_KEY,
            HTTP_VERSION
        };
        request.set(http::field::host, "api.opentripmap.com");
        request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        auto response = co_await send_request("api.opentripmap.com", "443", request);
        const std::map<std::tuple<std::string, std::string, bool>, bool> keys{
            {{"id", "ID", false}, false}, {{"name", "Name", true}, true}
        };
        std::stringstream ss;
        for (int i = 0; i < json::parse(response)["features"].size(); ++i) {
            ss << format_print(keys, json::parse(response)["features"][i]) + format_print(
                keys, json::parse(response)["features"][i]["properties"]) << std::endl;
            ss << co_await get_interesting_place_info(json::parse(response)["features"][i]["id"].get<std::string>()) <<
                    std::endl << std::endl;
        }
        co_return ss.str();
    }


    asio::awaitable<std::string> Finder::get_interesting_place_info(const std::string&id) {
        http::request<http::string_body> request{
            http::verb::get, "/0.1/en/places/xid/" + id + "?apikey=" + INTERESTING_PLACES_API_KEY,
            HTTP_VERSION
        };
        request.set(http::field::host, "api.opentripmap.com");
        request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        auto response = co_await send_request("api.opentripmap.com", "443", request);
        co_return json::parse(response)["wikipedia_extracts"]["text"].get<std::string>();
    }

    std::string Finder::format_print(const std::map<std::tuple<std::string, std::string, bool>, bool>&keys,
                                     const json&object) {
        std::stringstream ss;
        for (const auto&[key, display]: keys) {
            if (object.contains(std::get<0>(key))) {
                if (std::get<2>(key)) {
                    ss << std::get<1>(key) + ": ";
                }
                if (display) {
                    if (object[std::get<0>(key)].is_string()) {
                        ss << object[std::get<0>(key)].get<std::string>();
                    }
                    else if (object[std::get<0>(key)].is_number_float()) {
                        ss << std::setprecision(2) << object[std::get<0>(key)].get<double>();
                    }
                    else if (object[std::get<0>(key)].is_number_integer()) {
                        ss << object[std::get<0>(key)].get<int>();
                    }
                    ss << ", ";
                }
            }
        }
        std::string formatted = ss.str();
        // removing last ", "
        if (!formatted.empty()) {
            formatted.erase(formatted.end() - 2, formatted.end() - 1);
        }
        return formatted;
    }


    asio::awaitable<std::string> Finder::send_request(const std::string&host, const std::string&port,
                                                      http::request<http::string_body>&req) {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::resolver resolver{executor};
        ssl::context ctx(ssl::context::sslv23_client);
        ssl::stream<asio::ip::tcp::socket> stream(executor, ctx);
        const asio::ip::tcp::resolver::query query{host, port};
        auto [ec, it] = co_await resolver.async_resolve(query, as_tuple(asio::use_awaitable));
        if (ec) {
            std::cerr << "resolve: " + ec.what() << std::endl;
            co_return std::string();
        }
        if (auto [error] = co_await stream.lowest_layer().async_connect(*it, as_tuple(asio::use_awaitable)); error) {
            std::cerr << "connect: " + error.what() << std::endl;
            co_return std::string();
        }
        co_await stream.async_handshake(ssl::stream_base::handshake_type::client, asio::use_awaitable);
        if (auto [ec, n] = co_await http::async_write(stream, req, as_tuple(asio::use_awaitable)); ec) {
            std::cerr << "async write: " + ec.what() << std::endl;
            co_return std::string();
        }

        beast::flat_buffer buffer;
        http::response<http::string_body> response;
        if (auto [ec, n] = co_await http::async_read(stream, buffer, response, as_tuple(asio::use_awaitable));
            ec) {
            std::cerr << ec.what() << std::endl;
            co_return std::string();
        }
        co_return response.body();
    }
} // Places
