#ifndef WEATHER_HPP
#define WEATHER_HPP
#include "../Finder.hpp"


namespace Places {

    class Weather {
    public:
        static asio::awaitable<std::string> get_weather(const json& location);

    };
}



#endif //WEATHER_HPP
