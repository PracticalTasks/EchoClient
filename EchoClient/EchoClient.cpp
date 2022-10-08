#include "EchoClient.h"

#pragma warning(disable : 4996)

EchoClient::EchoClient(const uint16_t WRITE_PORT)
{
    //const char IP_SERV[] = "127.0.0.1";
    //servinfo = get_addrinfo(get_nameinfo(IP_SERV));
    servinfo = get_addrinfo("localhost");

    for (auto const* s = servinfo; s != nullptr; s = s->ai_next)
    {

        if (AF_INET == s->ai_family)
        {
            char ip[INET_ADDRSTRLEN];

            sockaddr_in* const sin = reinterpret_cast<sockaddr_in* const>(s->ai_addr);
            in_addr addr;
            addr.s_addr = *reinterpret_cast<const in_addr_t*>(&sin->sin_addr);

            sin->sin_family = AF_INET;
            sin->sin_port = htons(WRITE_PORT);

            std::cout << "Trying IP Address: " << inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN) << std::endl;
            socket_wrapper::Socket s{ AF_INET, SOCK_STREAM, IPPROTO_TCP };

            if (try_to_connect(s, reinterpret_cast<const sockaddr*>(sin), sizeof(sockaddr_in)))
            {
                client_sock = new socket_wrapper::Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

                if (init_client(WRITE_PORT) == EXIT_FAILURE)
                {
                    std::cout << "Connection ipv4 failed\n";
                    return;
                }
            }
            
            delete client_sock;

        }
        else if (AF_INET6 == s->ai_family)
        {
            char ip6[INET6_ADDRSTRLEN];

            sockaddr_in6* const sin = reinterpret_cast<sockaddr_in6* const>(s->ai_addr);

            sin->sin6_family = AF_INET6;
            sin->sin6_port = htons(WRITE_PORT);

            std::cout << "Trying IPv6 Address: " << inet_ntop(AF_INET6, &(sin->sin6_addr), ip6, INET6_ADDRSTRLEN) << std::endl;
            socket_wrapper::Socket s{ AF_INET6, SOCK_STREAM, IPPROTO_TCP };

            if (try_to_connect(s, reinterpret_cast<const sockaddr*>(sin), sizeof(sockaddr_in6)))
            {
                client_sock = new socket_wrapper::Socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
                if (init_client6(WRITE_PORT) == EXIT_FAILURE)
                {
                    std::cout << "Connection ipv6 failed\n";
                    return;
                }
            }

            delete client_sock;
        }
    }

    //if (servinfo->ai_family == AF_INET)
    //{
    //    if (init_client(WRITE_PORT) == EXIT_FAILURE)
    //    {
    //        std::cout << "Connection ipv4 failed\n";
    //        return;
    //    }
    //}
    //else if (servinfo->ai_family == AF_INET6)
    //{
    //    if (init_client6(WRITE_PORT) == EXIT_FAILURE)
    //    {
    //        std::cout << "Connection ipv6 failed\n";
    //        return;
    //    }
    //}

    write();
}

EchoClient::~EchoClient()
{
    freeaddrinfo(servinfo);
    //delete client_sock;
}

bool EchoClient::try_to_connect(socket_wrapper::Socket& s, const sockaddr* sa, size_t sa_size)
{
    if (connect(s, sa, sa_size) == -1)
    {
        std::cerr << "Connection failed: " << sock_wrap.get_last_error_string() << std::endl;
        return false;
    }

    s.close();
    return true;
}

//Метод для трансяции имени хоста в адресс
//Возвращает указатель на связанный список структур addrinfo содержащую информацию о хосте
addrinfo* EchoClient::get_addrinfo(const std::string& host_name)
{
    // Need for Windows initialization.
    //socket_wrapper::SocketWrapper sock_wrap;

    addrinfo* servinfo = nullptr;

    addrinfo hints =
    {
        .ai_flags = AI_CANONNAME,
        // Неважно, IPv4 или IPv6.
        .ai_family = AF_UNSPEC,
        // TCP stream-sockets.
        .ai_socktype = SOCK_STREAM,
        // Any protocol.
        .ai_protocol = 0
    };

    int status{};

    if ((status = getaddrinfo(host_name.c_str(), nullptr, &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return nullptr;
    }

    return servinfo;
}

//Метод для получения от сервера имени хоста по ip адресу
//Возвращает строку с именем хоста
char* EchoClient::get_nameinfo(const std::string& ip_addr)
{
    // Need for Windows initialization.
    //socket_wrapper::SocketWrapper sock_wrap;
    sockaddr_in pSockaddr;

    pSockaddr.sin_family = AF_INET;
    pSockaddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

    char host_name[NI_MAXHOST]{};

    //std::string hostName;
    int status{};
    if (status = getnameinfo((sockaddr*)&pSockaddr, sizeof(sockaddr), host_name, NI_MAXHOST, NULL, NULL, NULL) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return nullptr;
    }

    return host_name;
}

int EchoClient::init_client(const uint16_t WRITE_PORT)
{   
    std::cout << "Starting echo client ...\n";

    //client_sock = new socket_wrapper::Socket( AF_INET, SOCK_STREAM, NULL);

    if (!*client_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in* const sin = reinterpret_cast<sockaddr_in* const>(servinfo->ai_addr);
    in_addr addr;
    addr.s_addr = *reinterpret_cast<const in_addr_t*>(&sin->sin_addr);
    sin->sin_family = AF_INET;
    sin->sin_port = htons(WRITE_PORT);


    if (connect(*client_sock, (SOCKADDR*)sin, sizeof(sockaddr_in)))
    {
        std::cout << "Connection to Server is FAILED. Error #";
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected! ...\n";

    return EXIT_SUCCESS;
}

int EchoClient::init_client6(const uint16_t WRITE_PORT)
{
    std::cout << "Starting echo client ...\n";

    client_sock = new socket_wrapper::Socket(AF_INET6, SOCK_STREAM, NULL);

    if (!*client_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in6* const sin = reinterpret_cast<sockaddr_in6* const>(servinfo->ai_addr);

    sin->sin6_family = AF_INET6;
    sin->sin6_port = htons(WRITE_PORT);

    if (connect(*client_sock, (SOCKADDR*)sin, sizeof(sockaddr_in6)))
    {
        std::cout << "Connection to Server is FAILED. Error #";
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected! ...\n";

    return EXIT_SUCCESS;
}

int EchoClient::write()
{
    std::vector<char>buff(BUFF_SIZE);
    std::string send_mess;
    std::string send_buff;

    short packet_size = 0;

    while (true)
    {
        std::cout << "Your (Client) message to Server: ";
        fgets(buff.data(), buff.size(), stdin);

        send_mess = buff.data();

        packet_size = send(*client_sock, send_mess.c_str(), send_mess.size() - 1, 0);

        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can't send message to Server. Error #";
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }

        if (str_tolower(send_mess) == CMD_EXT)
        {
            shutdown(*client_sock, SD_BOTH);
            std::cout << "Stopping echo server ...\n";
            std::cout << "Disconected\n";
            return EXIT_SUCCESS;
        }

        packet_size = recv(*client_sock, buff.data(), buff.size(), 0);

        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can`t receiv message from Server. Error #";
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Server message: " << buff.data() << std::endl;
    }

    return EXIT_SUCCESS;
}

std::string EchoClient::str_tolower(const std::string& str)
{
    std::string str_lower;
    for (int i{}; i < str.size() - 1; ++i)
        str_lower += tolower(str[i]);

    return str_lower;
}

