#include "JSON_Formatter.hpp"

#include "../Config.hpp"

#include <sstream>


namespace Places {
    std::string JSON_Formatter::format_print(
        const std::map<std::tuple<json_key, displayable_key, display_key>, std::tuple<json_value_converter, display_value>>&keys,
        const nlohmann::json&object) {
        std::stringstream ss;
        bool remove_last_comma = false;
        for (const auto&[key,value]: keys) {
            if (auto json_key = std::get<0>(key); object.contains(json_key)) {
                // do not display empty string
                if (object[json_key].is_string() && !object[json_key].get<std::string>().empty() || !object[json_key].
                    is_string()) {
                    auto key_to_display = std::get<1>(key);
                    if (bool display_value = std::get<1>(value)) {
                        if (bool display_key = std::get<2>(key)) {
                            ss << key_to_display + ": ";
                        }
                        const auto&json_value = object[json_key];

                        if (const auto&function = std::get<0>(value); function != nullptr) {
                            ss << function(json_value);
                        }
                        else if (!json_value.empty()) {
                            ss << Config::get_string(json_value) << ", ";
                            remove_last_comma = true;
                        }
                    }
                }
            }
        }
        std::string formatted = ss.str();
        // removing last ", "
        if (!formatted.empty() && remove_last_comma) {
            formatted.erase(formatted.end() - 2, formatted.end() - 1);
        }
        return formatted;
    }


    std::string JSON_Formatter::get_value_type(const nlohmann::json&object) {
        if (object.is_null()) {
            return typeid(nullptr).name();
        }
        if (object.is_boolean()) {
            return typeid(bool).name();
        }
        if (object.is_number()) {
            return typeid(int).name();
        }
        if (object.is_number_float()) {
            return typeid(double).name();
        }
        if (object.is_string()) {
            return typeid(std::string).name();
        }
        return typeid(nullptr).name();
    }
}
