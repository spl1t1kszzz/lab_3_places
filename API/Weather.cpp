#include "Weather.hpp"


namespace Places {
    asio::awaitable<std::string> Weather::get_weather(const json& location) {
        co_return std::string();

    }
}
