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
	boost::asio::ip::tcp::socket socket; //�����
	boost::system::error_code ec; //���������� ��� ������
	boost::asio::streambuf streambuf; //�����
	std::unordered_set<std::shared_ptr<Session>> &clients; //������ ��������
	

	//������ �� ������ � �����
	void read_from_buffer(const boost::system::error_code& error, std::size_t bytes);

	//������ �� ������ ����� ������������ ��� ��������
	void read_from_buffer_name(const boost::system::error_code& error, std::size_t bytes);

	//�������� ��������� ���� ��������������
	void write_everyone(std::string& line);

	void _help();
	void _users();

public:
	Session(boost::asio::ip::tcp::socket&& socket, std::unordered_set<std::shared_ptr<Session>>& clients);

	std::string name; //��� ������������

	//���������� �������� ������������ ���������
	void async_write(std::string message);

	//������ ������
	void async_read();

	//������ ����� ������������ �� ������
	void async_read_name();

	//
	void recive_files();
};