#include "HTTP_Request.hpp"

namespace Places::API {
    asio::awaitable<std::string> HTTP_Request::send_request(const std::string& host, const std::string& port,
        http::request<http::string_body>& req) {

        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::resolver resolver{executor};
        ssl::context ctx(ssl::context::sslv23_client);
        ssl::stream<asio::ip::tcp::socket> stream(executor, ctx);
        const asio::ip::tcp::resolver::query query{host, port};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        auto [ec, it] = co_await resolver.async_resolve(query, as_tuple(asio::use_awaitable));
        if (ec) {
            std::cerr << "resolve: " + ec.what() << std::endl;
            co_return std::string();
        }
        if (auto [error] = co_await stream.lowest_layer().async_connect(*it, as_tuple(asio::use_awaitable)); error) {
            std::cerr << "connect: " + error.what() << std::endl;
            co_return std::string();
        }
        co_await stream.async_handshake(ssl::stream_base::handshake_type::client, asio::use_awaitable);
        if (auto [ec, n] = co_await http::async_write(stream, req, as_tuple(asio::use_awaitable)); ec) {
            std::cerr << "async write: " + ec.what() << std::endl;
            co_return std::string();
        }

        beast::flat_buffer buffer;
        http::response<http::string_body> response;
        if (auto [ec, n] = co_await http::async_read(stream, buffer, response, as_tuple(asio::use_awaitable));
            ec) {
            std::cerr << ec.what() << std::endl;
            co_return std::string();
            }
        co_return response.body();
    }
}
