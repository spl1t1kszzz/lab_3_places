#include "Finder.hpp"

namespace Places {
    using json = nlohmann::json;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace asio = boost::asio;
    namespace ssl = asio::ssl;
    using tcp = asio::ip::tcp;


    asio::awaitable<json> Finder::find_all_places(const std::string&place) {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::resolver resolver{executor};
        ssl::context ctx(ssl::context::sslv23_client);
        ssl::stream<asio::ip::tcp::socket> stream(executor, ctx);
        const asio::ip::tcp::resolver::query query{"graphhopper.com", "443"};
        auto [ec, it] = co_await resolver.async_resolve(query, as_tuple(asio::use_awaitable));
        if (ec) {
            std::cerr << "resolve: " + ec.what() << std::endl;
            co_return nullptr;
        }
        if (auto [error] = co_await stream.lowest_layer().async_connect(*it, as_tuple(asio::use_awaitable)); error) {
            std::cerr << "connect: " + error.what() << std::endl;
            co_return nullptr;
        }
        co_await stream.async_handshake(ssl::stream_base::client, asio::use_awaitable);
        http::request<http::string_body> req{
            http::verb::get, std::string("/api/1/geocode?q=" + place + "&locale=" + LOCALE + "&key=" + PLACES_API_KEY),
            HTTP_VERSION
        };
        req.set(http::field::host, "graphhopper.com");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        if (auto [ec, n] = co_await http::async_write(stream, req, as_tuple(asio::use_awaitable)); ec) {
            std::cerr << "async write: " + ec.what() << std::endl;
            co_return nullptr;
        }

        beast::flat_buffer buffer;
        http::response<http::string_body> response;
        if (auto [ec, n] = co_await http::async_read(stream, buffer, response, as_tuple(asio::use_awaitable));
            ec) {
            std::cerr << ec.what() << std::endl;
            co_return nullptr;
        }
        co_return json::parse(response.body())["hits"];
    }


    void Finder::find_places(const std::string&place) {
        asio::io_context io_context;
        co_spawn(io_context, [&place] { return start(place); }, asio::detached);
        io_context.run();
    }


    asio::awaitable<void> Finder::start(const std::string&place) {
        const auto json = co_await find_all_places(place);
        if (json == nullptr) {
            co_return;
        }
        for (int i = 0; i < json.size(); ++i) {
            std::cout << "Location " << i + 1 << ": " << format_place(json[i]) << std::endl << std::endl;
        }
        std::cout << "Choose location: ";
        int chosen_location;
        std::cin >> chosen_location;
        co_spawn(co_await asio::this_coro::executor,[&json, &chosen_location] {return get_weather(json[chosen_location - 1]);}, as_tuple(asio::detached));
    }

    asio::awaitable<void> Finder::get_weather(const json& location) {
        std::cout << location["point"] << std::endl;
        co_return;
    }

    std::string Finder::format_place(const json&place) {
        std::vector<std::string> keys{"postcode", "country", "state", "name"};
        std::stringstream ss;
        std::ranges::for_each(keys, [&place, &ss](const std::string&key) {
            if (place.contains(key)) {
                ss << std::string(place[key]) + ", ";
            }
        });
        std::string formatted_place = ss.str();
        formatted_place.erase(formatted_place.end() - 2, formatted_place.end() - 1);
        return formatted_place;
    }
} // Places
