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
    socket_wrapper::SocketWrapper sockWrap;
    socket_wrapper::Socket echoSock = { AF_INET, SOCK_DGRAM, 0 };

    if (!echoSock)
    {
        std::cerr << sockWrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in servAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(WRITE_PORT)
    };

    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    std::string buffer;
    char recvBuffer[256]{};
    int recvLen{};
    socklen_t clientAddressLen = sizeof(sockaddr_in);
    std::cout << "Starting echo client ...\n";


    while (true)
    {
        std::cin >> buffer;

        sendto(echoSock, buffer.c_str(), buffer.size(), 0, reinterpret_cast<const sockaddr*>(&servAddr),
            sizeof(servAddr));

        strToLower(buffer);

        if (buffer == CMD_EXT)
        {
            std::cout << "Stoped echo clients ...\n";
            break;
        }

        recvLen = recvfrom(echoSock, recvBuffer, sizeof(recvBuffer) - 1, 0,
            reinterpret_cast<sockaddr*>(&servAddr), &clientAddressLen);

        std::cout << recvBuffer << std::endl;
    }

    return EXIT_SUCCESS;
}

void EchoClient::strToLower(std::string& str)
{
    for (int i{}; i < str.size(); ++i)
        str[i] = tolower(str[i]);
}
