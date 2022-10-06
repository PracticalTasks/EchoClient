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
    std::cout << "Starting echo client ...\n";

    socket_wrapper::SocketWrapper sock_wrap;
    socket_wrapper::Socket client_sock = { AF_INET, SOCK_STREAM, NULL };
    const char IP_SERV[] = "127.0.0.1";

    if (!client_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(WRITE_PORT)
    };

    serv_addr.sin_addr.s_addr = inet_addr(IP_SERV);

    if (connect(client_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)))
    {
        std::cout << "Connection to Server is FAILED. Error #";
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected! ...\n";

    //std::string buffer;
    //char recv_buffer[256]{};
    //int recv_len{};
    //socklen_t client_addrlen = sizeof(sockaddr_in);

    std::vector<char>buff(BUFF_SIZE);
    std::string send_mess;
    std::string send_buff;

    short packet_size = 0;

    while (true)
    {
        std::cout << "Your (Client) message to Server: ";
        //std::cin >> send_mess;
        fgets(buff.data(), buff.size(), stdin);

        //send_buff.reserve(send_mess.size() + 2);
        //uint8_t sz_l = send_mess.size();
        //uint8_t sz_h = (send_mess.size() & 0xFFFF) >> 8;

        //send_buff.at(0) = sz_l;
        //send_buff.at(1) = sz_h;
        //send_buff.insert(2, send_mess);

        send_mess = buff.data();

        packet_size = send(client_sock, send_mess.c_str(), send_mess.size() - 1, 0);

        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can't send message to Server. Error #";
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }

        if (send_mess == "exit\n")
        {
            shutdown(client_sock, SD_BOTH);
            std::cout << "Stopping echo server ...\n";
            std::cout << "Disconected\n";
            return 0;
        }

        packet_size = recv(client_sock, buff.data(), buff.size(), 0);

        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can`t receiv message from Server. Error #";
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Server message: " << buff.data() << std::endl;

        //std::cin >> buffer;

        //sendto(client_sock, buffer.c_str(), buffer.size(), 0, reinterpret_cast<const sockaddr*>(&serv_addr),
        //    sizeof(serv_addr));

        //str_tolower(buffer);

        //if (buffer == CMD_EXT)
        //{
        //    std::cout << "Stoped echo clients ...\n";
        //    break;
        //}

        //recv_len = recv(client_sock, recv_buffer, sizeof(recv_buffer) - 1, 0);

        //std::cout << recv_buffer << std::endl;


    }

    return EXIT_SUCCESS;
}

void EchoClient::str_tolower(std::string& str)
{
    for (int i{}; i < str.size(); ++i)
        str[i] = tolower(str[i]);

}

