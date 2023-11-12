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
            http::verb::get, std::string("/api/1/geocode?q=" + place + "&locale=" + LOCALE + "&key=" + API_KEY),
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
            std::cout << "Location " << i + 1 << ": " << json[i] << std::endl << std::endl;
        }
    }
} // Places
