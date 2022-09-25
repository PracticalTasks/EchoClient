#include "EchoClient.h"

#pragma warning(disable : 4996)

EchoClient::EchoClient(const uint16_t WRITE_PORT)
{
    write(WRITE_PORT);
}

EchoClient::~EchoClient()
{
}

// Trim from end (in place).
//inline std::string& EchoClient::rtrim(std::string& s)
//{
//    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base());
//    return s;
//}

int EchoClient::write(const uint16_t WRITE_PORT)
{
    socket_wrapper::SocketWrapper sock_wrap;
    socket_wrapper::Socket echoSock = { AF_INET, SOCK_DGRAM, 0 };;

    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(WRITE_PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true)
    {
        std::string buffer;

        std::cout << "Starting echo client ...\n";
        std::cout << "Enter your message ...\n";
        std::cin >> buffer;

        sendto(echoSock, buffer.c_str(), buffer.size(), 0, reinterpret_cast<const sockaddr*>(&servAddr),
            sizeof(servAddr));
    }


    //if (!echoSock)
    //{
    //    std::cerr << sock_wrap.get_last_error_string() << std::endl;
    //    return EXIT_FAILURE;
    //}

    //sockaddr_in addr =
    //{
    //    .sin_family = PF_INET,
    //    .sin_port = htons(READ_PORT),
    //};

    //addr.sin_addr.s_addr = INADDR_ANY;

    //if (bind(echoSock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    //{
    //    std::cerr << sock_wrap.get_last_error_string() << std::endl;
    //    // Socket will be closed in the Socket destructor.
    //    return EXIT_FAILURE;
    //}

    //char buffer[256]{};

    //// socket address used to store client address
    //struct sockaddr_in client_address = { 0 };
    //socklen_t client_address_len = sizeof(sockaddr_in);
    //ssize_t recv_len = 0;

    //std::cout << "Running echo server...\n" << std::endl;
    //char client_address_buf[INET_ADDRSTRLEN];

    //while (true)
    //{
    //    // Read content into buffer from an incoming client.
    //    recv_len = recvfrom(echoSock, buffer, sizeof(buffer) - 1, 0,
    //        reinterpret_cast<sockaddr*>(&client_address),
    //        &client_address_len);

    //    if (recv_len > 0)
    //    {
    //        buffer[recv_len] = '\0';
    //        std::cout
    //            << "Client with address "
    //            << inet_ntop(AF_INET, &client_address.sin_addr, client_address_buf, sizeof(client_address_buf) / sizeof(client_address_buf[0]))
    //            << ":" << ntohs(client_address.sin_port)
    //            << " sent datagram "
    //            << "[length = "
    //            << recv_len
    //            << "]:\n'''\n"
    //            << buffer
    //            << "\n'''"
    //            << std::endl;
    //        std::string tmpStr = buffer;
    //        std::string cmdStr = rtrim(tmpStr);
    //        if (cmdStr == "exit")
    //        {
    //            std::cout << "Stoped echo server ...\n";
    //            break;
    //        }
    //            
    //        sendto(echoSock, buffer, recv_len, 0, reinterpret_cast<const sockaddr*>(&client_address),
    //            client_address_len);
    //    }

    //    std::cout << std::endl;
    //}

    return EXIT_SUCCESS;
}