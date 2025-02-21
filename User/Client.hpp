#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <optional>
#include <thread>
#include <fstream>

namespace B = boost::asio;
using tcp = B::ip::tcp;
using error_code = boost::system::error_code;

class Client {

public:

    Client(B::io_context& io_context);

    //����������� �������� ���������
    void async_read();

    //������ � ������
    void read_from_buffer(const error_code& error, std::size_t bytes);

    //�������� ��������� �� ������
    void async_write(std::string message);

    //������� ����
    void input();

    //���� �����
    void input_name();

    //�������� �����
    void send_file(std::string&& file_name);

private:
    boost::system::error_code ec;
    tcp::socket socket;
    B::streambuf streambuf;
    B::io_context& io_context;
    std::string name;
    bool can_chat = false;
};