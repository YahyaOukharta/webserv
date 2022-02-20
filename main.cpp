#include <iostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

int main(int ac, char **av)
{
    // create Socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        std::cout << "Error creating socket" << std::endl;
        return (1);
    }
    std::cout << "Socket = " << sock << std::endl;

    // set socket options
    int sockopt_val = 1;
    if ( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt_val, sizeof(sockopt_val)) )
    {
        // //perror("setsockopt");
        std::cout << "Error setting socket options" << std::endl;
        return (1);
    }

    // set socket port. and address ?
    int port = 8080;
    struct sockaddr_in address;
    int address_size = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&address, sizeof(address)))
    {
        // //perror("bind");
        std::cout << "Error bind socket to port" << std::endl;
    }
    std::cout << "Socket binded to port " << port << std::endl;

    // listening
    if(listen(sock, 3)){
        // //perror("listen");
        std::cout << "Error listening" << std::endl;
    }
    std::cout << "Listening on port " << port << std::endl;

    // Accept
 

    //handle ?
    while (1)
    {
        int new_sock;
        if(( new_sock = accept(sock, (struct sockaddr *)&address, (socklen_t*)&address_size)) < 0)
        {
            // //perror("accept");
            std::cout << "Error accepting" << std::endl;
        }
        std::cout << "accepted connection on port " << port << std::endl;
        char buffer[102400] = {0};
        int valread = read( new_sock , buffer, 102400);
        if(valread == 0) //EOF
            break;
        std::cout << buffer << std::endl;
        send(new_sock, "HELLOOOO\n", 10, 0);
    }
    std::cout << "done" << std::endl;
  
    return (0);
}