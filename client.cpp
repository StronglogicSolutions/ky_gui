#include <client.hpp>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <thread>
#include <cstring>
#include <QDebug>
#include <QByteArray>
#include <string>
#include <iostream>

void input_loop(char buffer[]) {
    qDebug() << "Enter message: ";
    for (;;) {
        std::cin >> buffer;
        if (buffer[0] != 0) {
            return;
        }
    }
}
//hostCombo(new QComboBox),
//    portLineEdit(new QLineEdit),
//    getFortuneButton(new QPushButton(tr("Get Fortune"))),
//    tcpSocket(new QTcpSocket(this)) {
//    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

//    in.setDevice(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_0);

//    QPushButton *button = new QPushButton("Push");
//    parent->layout()->addWidget(button);
//    connect(button, &QPushButton::clicked, this, &Client::clicked);
//    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readFortune);
//}


Client::Client(QWidget *parent, int count, char** arguments) : QDialog(parent), argc(count), argv(arguments), m_client_socket_fd(-1) {}

    void Client::handleMessages() {
        char receive_buffer[2048];
        for (;;) {
            memset(receive_buffer, 0, 2048);
            int bytes_received = 0;
            bytes_received = recv(m_client_socket_fd, receive_buffer, 2048 - 2, 0);
            receive_buffer[2047] = 0;
            if (bytes_received == 0) {
                break;
            }
            emit Client::messageReceived(QString::fromUtf8(receive_buffer, 2048));
        }
        memset(receive_buffer, 0, 2048);
        ::close(m_client_socket_fd);
    }

    int Client::start() {
        m_client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_client_socket_fd != -1) {
            sockaddr_in server_socket;
            char* end;
            server_socket.sin_family = AF_INET;
            auto port_value = strtol(argv[2], &end, 10);
            if (port_value < 0 || end == argv[2]) {
                return 1;
            }

            int socket_option = 1;
            // Free up the port to begin listening again
            setsockopt(m_client_socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option,
                       sizeof(socket_option));

            server_socket.sin_port = htons(port_value);
            inet_pton(AF_INET, argv[1], &server_socket.sin_addr.s_addr);

            if (::connect(m_client_socket_fd, (sockaddr*)&server_socket,
                        sizeof(server_socket)) != -1) {
//                std::thread t(handleMessages);
            } else {
                qDebug() << errno;
                ::close(m_client_socket_fd);
            }
            return 0;
        }
    }

void Client::requestNewFortune() {
    QMessageBox::information(this, tr("Jigga Client"),
                             tr("Gibs me dat fortune, punk!"));
}

void Client::sendMessage(const QString& s) {
    if (m_client_socket_fd != -1) {
        QByteArray byte_array = s.toLocal8Bit();
        ::send(m_client_socket_fd, byte_array.data(), byte_array.size(), 0);
    } else {
        qDebug() << "You must first open a connection";
    }
}
