#pragma once

#include <vector>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_set>
#include <streambuf>

class Session : public std::enable_shared_from_this<Session> {

protected:
	boost::asio::ip::tcp::socket socket; //Сокет
	boost::system::error_code ec; //Переменная для ошибок
	boost::asio::streambuf streambuf; //Буфер
	std::unordered_set<std::shared_ptr<Session>> &clients; //Список клиентов
	

	//Чтение из потока в буфер
	void read_from_buffer(const boost::system::error_code& error, std::size_t bytes);

	//Чтение из потока имени пользователя при создании
	void read_from_buffer_name(const boost::system::error_code& error, std::size_t bytes);

	//Отправка сообщения всем поользователям
	void write_everyone(std::string& line);

	void _help();
	void _users();

public:
	Session(boost::asio::ip::tcp::socket&& socket, std::unordered_set<std::shared_ptr<Session>>& clients);

	std::string name; //Имя пользователя

	//Отправляет текущему пользователю сообщение
	void async_write(std::string message);

	//Чтение потока
	void async_read();

	//Чтение имени пользователя из потока
	void async_read_name();

	//
	void recive_files();
};