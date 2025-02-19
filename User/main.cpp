#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <optional>
#include <thread>

namespace B = boost::asio;
using tcp = B::ip::tcp;
using error_code = boost::system::error_code;

class Client {

public:

    Client(B::io_context& io_context) : io_context{ io_context }, socket(io_context) {
        auto address = B::ip::make_address_v4("192.168.180.31", ec);
        if (ec) {
            std::cerr << "Ошибка: " << ec.message() << std::endl;
            std::exit(1);
        }

        tcp::endpoint endpoint(address, 53888);

        socket.connect(endpoint, ec);
        if (ec == B::error::connection_refused) {
            std::cerr << "Сервер не ответил." << std::endl;
            std::exit(1);
        }
        if (ec) {
            std::cerr << "Ошибка подключения: " << ec.message() << std::endl;
            std::exit(1);
        }
    }

    void async_read() {
        B::async_read_until(socket, streambuf, '\f',
            [this](error_code error, std::size_t bytes) {
                read_from_buffer(error, bytes);
            });
    }

    void read_from_buffer(const error_code& error, std::size_t bytes)
    {
        if (error) {
            if (error == B::error::eof) {
                std::cerr << "Сервер разорвал соединение." << std::endl;
            }
            else {
                std::cout << "Ошибка: " << error.message() << std::endl;
            }
            return;
        }

        std::istream input(&streambuf);
        std::string line;
        std::getline(input, line, '\f');
        if (can_chat)
        {
            int size_name = name.size() + 1;
            std::string delete_name(size_name, ' ');
            std::cout << '\r' << delete_name << '\r' << line << name << ": " << std::flush;
        } 
        else
        {
            std::cout << line;
        }

        async_read();
    }

    void async_write(std::string message) {
        B::async_write(socket, B::buffer(message.data(), message.size()), 
            [this](error_code error, std::size_t bytes) {
            if (error) {
                std::cerr << "Ошибка отправки: " << error.message() << std::endl;
                return;
            }
            });
    }

    void input() {
        can_chat = true;
        std::thread([this]() {
            std::string message;
            while (true) {
                std::cout << name << ": ";
                std::getline(std::cin, message);
                async_write(message+'\n');
            }
            }).detach();
    }

    void input_name() {
        std::thread([this]() {
            std::string message;
            std::getline(std::cin, message);
            async_write(message + '\n');
            name = message;
            input();
            }).detach();
    }

private:
    boost::system::error_code ec;
    tcp::socket socket;
    B::streambuf streambuf;
    B::io_context& io_context;
    std::string name;
    bool can_chat = false;
};

int main() {
    setlocale(LC_ALL, "RU");
    B::io_context io_context;
    Client client{io_context};
    client.async_read();
    client.input_name();
    io_context.run();
}