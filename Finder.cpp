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
            co_return;
        }
        // 1st string - key in json, 2nd key - key to display
        for (int i = 0; i < json.size(); ++i) {
            std::cout << "Location " << i + 1 << ": " << JSON_Formatter::format_print(Config::LOCATION_KEYS, json[i]) <<
                    std::endl <<
                    std::endl;
        }
        std::cout << "Choose location: ";
        int chosen_location;
        std::cin >> chosen_location;
        std::stringstream result;
        result << std::string("Weather in ") + json[chosen_location - 1]["name"].get<std::string>() + ": " + co_await
                API::Weather::get_weather(json[chosen_location - 1]);
        result << std::endl << co_await API::Interesting_place::get_interesting_places(json[chosen_location - 1]);
        std::cout << result.str() << std::flush;
    }


} // Places
