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
	int write(const uint16_t WRITE_PORT);
	std::string str_tolower(const std::string& str);

private:
	const std::string CMD_EXT = "exit";
	const uint16_t BUFF_SIZE = 256;
};
