#ifndef JSON_FORMATTER_HPP
#define JSON_FORMATTER_HPP

#include "json.hpp"

namespace Places {
    class JSON_Formatter {
    public:
        static std::string format_print(const std::map<std::tuple<std::string, std::string, bool>, std::tuple<std::function<std::string(const nlohmann::json&)>, bool>>&keys,
                                        const nlohmann::json&object);

        static std::string get_value_type(const nlohmann::json&object);
    };
}


#endif //JSON_FORMATTER_HPP
