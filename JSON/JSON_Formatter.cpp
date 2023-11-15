#include "JSON_Formatter.hpp"


namespace Places {
    std::string JSON_Formatter::format_print(
        const std::map<std::tuple<std::string, std::string, bool>, bool>&keys,
        const nlohmann::json&object) {
        std::stringstream ss;
        for (const auto&[key, display_value]: keys) {
            if (auto json_key = std::get<0>(key); object.contains(json_key)) {
                auto key_to_display = std::get<1>(key);
                if (display_value) {
                    if (bool display_key = std::get<2>(key)) {
                        ss << key_to_display + ": ";
                    }
                    if (object[json_key].is_string()) {
                        auto value = object[json_key].get<std::string>();
                        ss << (value.empty() ? "no" + key_to_display : value);
                    }
                    else if (object[json_key].is_number_float()) {
                        ss << std::setprecision(2) << object[json_key].get<double>();
                    }
                    else if (object[json_key].is_number_integer()) {
                        ss << object[json_key].get<int>();
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
}
