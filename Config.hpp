#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <map>

#include "JSON/json.hpp"
#include "JSON/JSON_Formatter.hpp"


namespace Places {
    using json_value_converter = std::function<std::string(const nlohmann::json&)>;
    using json_key = std::string;
    using displayable_key = std::string;
    using display_value = bool;
    using display_key = bool;
    struct Config {
        constexpr static auto LIMIT_OF_INT_PLACES = 5;

        constexpr static auto LIMIT_OF_LOCATIONS = 10;

        constexpr static auto LOCALE = "ru";

        constexpr static auto HTTP_VERSION = 11;

        constexpr static auto RADIUS = 2000;

        constexpr static auto LOCATIONS_API_KEY = "8f06ab6d-727d-453d-a63c-db843b782335";

        constexpr static auto INTERESTING_PLACES_API_KEY = "5ae2e3f221c38a28845f05b6bbd0c626598864236b0c0fcf6db566b3";



        inline static json_value_converter default_value_format = [](const std::string&x) {
            return x + '\n';
        };
        inline static json_value_converter get_string = [](const nlohmann::json&object) {
            using namespace std;
            const auto&val_type = JSON_Formatter::get_value_type(object);
            if (val_type == typeid(int).name()) {
                return to_string(object.get<int>());
            }
            if (val_type == typeid(double).name()) {
                return to_string(object.get<double>());
            }
            if (val_type == typeid(string).name()) {
                return object.get<string>();
            }
            return string();
        };

        inline static json_value_converter presssure_converter = [](const nlohmann::json&object) {
            using namespace std;
            const auto val_type = JSON_Formatter::get_value_type(object);
            if (val_type == typeid(int).name()) {
                return default_value_format(to_string(object.get<int>() * 0.75));
            }
            if (val_type == typeid(string).name()) {
                return default_value_format(to_string(stoi(object.get<string>()) * 0.75));
            }
            return string();
        };

        inline static json_value_converter temperature_converter = [](const nlohmann::json&object) {
            using namespace std;
            const auto val_type = JSON_Formatter::get_value_type(object);
            if (val_type == typeid(int).name()) {
                return default_value_format(to_string(object.get<int>()));
            }
            if (val_type == typeid(string).name()) {
                return default_value_format(object.get<string>());
            }
            return string();
        };

        inline const static
        std::map<std::tuple<json_key, displayable_key, display_key>, std::tuple<json_value_converter, display_value>>
        LOCATION_KEYS{
            {{"postcode", "Postcode", false}, {nullptr, true}},
            {{"country", "Country", false}, {nullptr, true}},
            {{"state", "State", false}, {nullptr, true}},
            {{"name", "name", false}, {nullptr, true}}
        };


        inline const static
        std::map<std::tuple<json_key, displayable_key, display_key>, std::tuple<json_value_converter, display_value>>
        WEATHER_KEYS{
            {{"temp", "Temerature (C)", true}, {temperature_converter, true}},
            {{"feels_like", "Feels like (C)", true}, {temperature_converter, true}},
            {{"humidity", "Humidity (%)", true}, {temperature_converter, true}},
            {{"pressure", "Pressure (mmHg)", true}, {presssure_converter, true}}
        };

        inline const static
        std::map<std::tuple<json_key, displayable_key, display_key>, std::tuple<json_value_converter, display_value>>
        INT_PLACE_KEYS{
            {{"id", "ID", false}, {nullptr, false}},
            {{"name", "Name", true}, {nullptr, true}},
            {{"address", "Address", true}, {nullptr, true}}
        };
    };
}


#endif //CONFIG_HPP
