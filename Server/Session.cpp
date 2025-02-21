#include "Session.hpp"


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

    if (line == "/!!!!") {
        //std::shared_ptr<Session1> session1 = std::make_shared<Session1>();
    }
    else if (line.starts_with("/help")) {
        _help();
    }
    else if (line.starts_with("/users")) {
        _users();
    }
    else {
        write_everyone(line);
    }
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
    clients.insert(shared_from_this());

    std::cout << socket.remote_endpoint().address().to_string() << " установил имя " << line << std::endl;

    async_read();
}

void Session::write_everyone(std::string& line)
{
    for (auto& i : clients) {
        if (i != shared_from_this())
        {
            i->async_write(name + ": " + line + '\n');
        }
    }

    std::cout << name << ": " << line << std::endl;

    async_read();
}

void Session::_help()
{
    std::string list = "\nДоступные команды:\n";
    list += "/users - список пользователей\n";
    list += "/make local - создать локальный чат\n";
    list += "/exit - выход\n\n";

    async_write(list);
    async_read();
}

void Session::_users()
{
    int count = 1;
    std::string users = "\nСписок пользователей:\n";
    for (const auto& i : clients) {
        users += std::to_string(count) + '.' + i->name + '\n';
        count++;
    }
    users += '\n';
    async_write(users);
    async_read();
}
