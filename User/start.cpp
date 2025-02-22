#include "Client.hpp"

// #include <QApplication>
// #include <QWidget>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    setlocale(LC_ALL, "RU");
    B::io_context io_context;
    Client client{io_context};
    client.async_read();
    client.input_name();
    io_context.run();

    std::cout << "Hello!" << std::endl;

    // QWidget w;
    // w.setFixedSize(600, 400);
    // w.show();

    //return a.exec();

    return 0;
}
