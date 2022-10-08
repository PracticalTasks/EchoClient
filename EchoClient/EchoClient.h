#pragma once
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>


class EchoClient
{
public:
	EchoClient(const uint16_t WRITE_PORT);
	~EchoClient();

private:
	addrinfo* get_addrinfo(const std::string& host_name);
	char* get_nameinfo(const std::string& ip_addr);
	int init_client(const uint16_t WRITE_PORT);
	int init_client6(const uint16_t WRITE_PORT);
	int write();
	std::string str_tolower(const std::string& str);
	bool try_to_connect(socket_wrapper::Socket& s, const sockaddr* sa, size_t sa_size);

private:
	socket_wrapper::SocketWrapper sock_wrap;
	socket_wrapper::Socket* client_sock;
	addrinfo* servinfo;
	const std::string CMD_EXT = "exit";
	const uint16_t BUFF_SIZE = 256;

};
