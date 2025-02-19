#include "Session.hpp"
#include <streambuf>

namespace B = boost::asio;
using tcp = B::ip::tcp;
using error_code = boost::system::error_code;

Session::Session(tcp::socket&& socket, std::unordered_set<std::shared_ptr<Session>>& clients)
    : socket{std::move(socket)}, clients{clients}
{

}

void Session::async_write(std::string message)
{
    message += '\f';
    B::async_write(socket, B::buffer(message.data(), message.size()),
        [self = shared_from_this()](error_code error, std::size_t bytes) {
            if (error) {
                std::cerr << error.message() << std::endl;
            }
        });
}

void Session::async_read() {
    B::async_read_until(socket, streambuf, '\n',
        [self = shared_from_this()] (error_code error, std::size_t bytes) {
            self->read_from_buffer(error, bytes);
        });
}

void Session::async_read_name()
{
    B::async_read_until(socket, streambuf, '\n',
        [self = shared_from_this()](error_code error, std::size_t bytes) {
            self->read_from_buffer_name(error, bytes);
        });
}

void Session::read_from_buffer(const error_code& error, std::size_t bytes)
{
    if (error) {
        if (error == B::error::eof || error == B::error::connection_reset) {
            std::cerr << name << " отключился." << std::endl;
        }
        else {
            std::cout << "Ошибка: " << error.message() << std::endl;
        }
        clients.erase(shared_from_this());
        socket.close();
        return;
    }

    std::istream input(&streambuf);
    std::string line;
    std::getline(input, line);

    for (auto &i : clients) {
        if (i != shared_from_this() )
        {
            i->async_write(name + ": " + line + '\n');
        }
    }

    std::cout << name << ": " << line << std::endl;

    async_read();
}

void Session::read_from_buffer_name(const boost::system::error_code& error, std::size_t bytes)
{
    if (error) {
        if (error == B::error::eof || error == B::error::connection_reset) {
            std::cerr << socket.remote_endpoint().address().to_string() << " отключился." << std::endl;
        }
        else {
            std::cout << "Ошибка: " << error.message() << std::endl;
        }
        return;
    }

    std::istream input(&streambuf);
    std::string line;
    std::getline(input, line);

    name = line;

    std::cout << socket.remote_endpoint().address().to_string() << " установил имя " << line << std::endl;

    async_read();
}
