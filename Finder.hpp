// ReSharper disable CppUnusedIncludeDirective
#ifndef FINDER_HPP
#define FINDER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl.hpp>
#include "json.hpp"


namespace Places {
    using json = nlohmann::json;
    namespace asio = boost::asio;

    class Finder {
        constexpr static auto API_KEY = "8f06ab6d-727d-453d-a63c-db843b782335";

        constexpr static auto LOCALE = "en";

        constexpr static auto HTTP_VERSION = 11;

        static asio::awaitable<json> find_all_places(const std::string&place);

        static asio::awaitable<void> start(const std::string& place);

    public:
        static void find_places(const std::string&place);
    };
} // Places

#endif //FINDER_HPP
