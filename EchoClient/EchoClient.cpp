#include "EchoClient.h"

#pragma warning(disable : 4996)

EchoClient::EchoClient(const uint16_t WRITE_PORT)
{
    write(WRITE_PORT);
}

EchoClient::~EchoClient()
{
}

int EchoClient::write(const uint16_t WRITE_PORT)
{
    socket_wrapper::SocketWrapper sock_wrap;
    socket_wrapper::Socket echo_sock = { AF_INET, SOCK_DGRAM, 0 };

    if (!echo_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(WRITE_PORT)
    };

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    std::string buffer;
    char recv_buffer[256]{};
    int recv_len{};
    socklen_t client_addrlen = sizeof(sockaddr_in);
    std::cout << "Starting echo client ...\n";


    while (true)
    {
        std::cin >> buffer;

        sendto(echo_sock, buffer.c_str(), buffer.size(), 0, reinterpret_cast<const sockaddr*>(&serv_addr),
            sizeof(serv_addr));

        str_tolower(buffer);

        if (buffer == CMD_EXT)
        {
            std::cout << "Stoped echo clients ...\n";
            break;
        }

        recv_len = recv(echo_sock, recv_buffer, sizeof(recv_buffer) - 1, 0);

        std::cout << recv_buffer << std::endl;
    }

    return EXIT_SUCCESS;
}

void EchoClient::str_tolower(std::string& str)
{
    for (int i{}; i < str.size(); ++i)
        str[i] = tolower(str[i]);

}
