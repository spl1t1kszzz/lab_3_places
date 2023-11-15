#ifndef INTERESTING_PLACE_HPP
#define INTERESTING_PLACE_HPP


#include <boost/asio/awaitable.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "../JSON/json.hpp"
#include "../Config.hpp"
#include "HTTP_Request.hpp"
#include "../JSON/JSON_Formatter.hpp"

namespace Places::API {
    using json = nlohmann::json;
    namespace asio = boost::asio;
    namespace beast = boost::beast;
    namespace http = beast::http;

    class Interesting_place {
    public:
        static asio::awaitable<json> find_all_locations(const std::string&location);

        static asio::awaitable<std::string> get_interesting_places(const json&location);

        static asio::awaitable<std::string> get_interesting_place_info(const std::string&id);
    };
}


#endif //INTERESTING_PLACE_HPP
