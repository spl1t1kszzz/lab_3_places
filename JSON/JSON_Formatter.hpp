#ifndef JSON_FORMATTER_HPP
#define JSON_FORMATTER_HPP

#include "json.hpp"

namespace Places {
    class JSON_Formatter {
    public:
        static std::string format_print(const std::map<std::tuple<std::string, std::string, bool>, bool>&keys,
                                        const nlohmann::json&object);
    };
}


#endif //JSON_FORMATTER_HPP
