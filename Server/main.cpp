#include "Server.hpp"

int main() {
	setlocale(LC_ALL, "RU");
	B::io_context io_context;
	int port = 53888;
	Server server(io_context, port);
	server.async_accept();
	io_context.run();
}