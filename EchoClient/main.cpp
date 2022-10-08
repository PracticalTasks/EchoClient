#include "EchoClient.h"

int main(int argc, char const *argv[])
{
//    if (argc != 2)
//{
//    std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
//    return EXIT_FAILURE;
//}
//
//    const int WRITE_PORT { std::stoi(argv[1]) };
    const int WRITE_PORT = 7777;
    EchoClient echo(WRITE_PORT);
}