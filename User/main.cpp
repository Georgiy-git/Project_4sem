#include "Client.hpp"

int main() {
    setlocale(LC_ALL, "RU");
    B::io_context io_context;
    Client client{io_context};
    client.async_read();
    client.input_name();
    io_context.run();
}