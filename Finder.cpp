#include "Finder.hpp"


namespace Places {
    void Finder::find_places(const std::string&place) {
        asio::io_context io_context;
        co_spawn(io_context, [&place] { return start(place); }, asio::detached);
        io_context.run();
    }


    asio::awaitable<void> Finder::start(const std::string&location) {
        const auto json = co_await API::Interesting_place::find_all_locations(location);
        if (json == nullptr) {
            std::cout << "No locations found :(" << std::endl;
            co_return;
        }
        for (int i = 0; i < json.size(); ++i) {
            std::cout << "Location " << i + 1 << ": " << JSON_Formatter::format_print(Config::LOCATION_KEYS, json[i]) <<
                    std::endl << std::endl;
        }
        std::cout << "Choose location:";
        int chosen_location;
        std::cin >> chosen_location;
        std::stringstream result;

        auto chosen_json = json[chosen_location - 1];
        result << std::string("\n\nWeather in ") + chosen_json["name"].get<std::string>() + ": " << std::endl << co_await
                API::Weather::get_weather(chosen_json) << std::endl << std::endl;
        result << std::endl << co_await API::Interesting_place::get_interesting_places(chosen_json);
        std::cout << result.str() << std::endl;
        co_return;
    }
} // Places
