#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <map>


namespace Places {
    struct Config {
        constexpr static auto LIMIT_OF_INT_PLACES = 5;

        constexpr static auto LOCALE = "en";

        constexpr static auto HTTP_VERSION = 11;

        constexpr static auto RADIUS = 2000;

        constexpr static auto LOCATIONS_API_KEY = "8f06ab6d-727d-453d-a63c-db843b782335";

        constexpr static auto INTERESTING_PLACES_API_KEY = "5ae2e3f221c38a28845f05b6bbd0c626598864236b0c0fcf6db566b3";

        inline const static std::map<std::tuple<std::string, std::string, bool>, bool> LOCATION_KEYS{
            {{"postcode", "Postcode", false}, true}, {{"country", "Country", false}, true},
            {{"state", "State", false}, true},
            {{"name", "name", false}, true}
        };

        inline const static std::map<std::tuple<std::string, std::string, bool>, bool> WEATHER_KEYS{
            {{"temp", "Temerature (C)", true}, true}, {{"feels_like", "Feels like (C)", true}, true},
            {{"humidity", "Humidity (%)", true}, true}, {{"pressure", "Pressure (hPa)", true}, true}
        };

        inline const static std::map<std::tuple<std::string, std::string, bool>, bool> INT_PLACE_KEYS{
            {{"id", "ID", false}, false}, {{"name", "Name", true}, true}, {{"address", "Address", true}, true}
        };
    };
}


#endif //CONFIG_HPP
