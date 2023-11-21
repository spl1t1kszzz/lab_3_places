#include "JSON_Formatter.hpp"

#include <iostream>


namespace Places {
    std::string JSON_Formatter::format_print(const std::map<std::tuple<std::string, std::string, bool>, std::tuple<std::function<std::string(const nlohmann::json&)>, bool>>&keys,
                                        const nlohmann::json&object) {
        std::stringstream ss;
        bool remove_last_comma = false;
        for (const auto&[key,value]: keys) {

            if (auto json_key = std::get<0>(key); object.contains(json_key)) {
                auto key_to_display = std::get<1>(key);
                if (bool display_value = std::get<1>(value)) {
                    if (bool display_key = std::get<2>(key)) {
                        ss << key_to_display + ": ";
                    }
                    const auto& json_value = object[json_key];
                    const auto& function = std::get<0>(value);

                    if (function != nullptr) {
                        ss << function(json_value);
                    } else if (!json_value.empty()) {
                        ss << json_value << ", ";
                        remove_last_comma = true;
                    }

                    // if (object[json_key].is_string()) {
                    //     auto json_value = object[json_key].get<std::string>();
                    //     ss << (json_value.empty() ? "No " + key_to_display : json_value);
                    // }
                    // else if (object[json_key].is_number_float()) {
                    //     ss << std::setprecision(2) << object[json_key].get<double>();
                    // }
                    // else if (object[json_key].is_number_integer()) {
                    //     ss << object[json_key].get<int>();
                    // }
                    //ss << ", ";
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


    std::string JSON_Formatter::get_value_type(const nlohmann::json& object) {
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
