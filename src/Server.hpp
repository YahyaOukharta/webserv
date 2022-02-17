#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include "./ServerConfig.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include "Request.hpp"
class Server
{
	private:
		ServerConfig conf;
		int sock;
		struct sockaddr_in address;
		

	public:
		Server(){

		};

		Server(ServerConfig c){
			conf = c;
		};
		Server(std::string name, std::string host, int port, int backlog){
			conf = ServerConfig(name, host, port, backlog);
		};
		Server( Server const & src ){
			*this = src;
		}
		~Server(){}

		Server &		operator=( Server const & rhs ){
			conf = rhs.getConfig();
			return *this;
		}

		//Config
		ServerConfig getConfig()const {
			return conf;
		}
		void setConfig(ServerConfig const & c) {
			conf = c;
		}

		//validate conf
		bool isValidConf(){
			if (conf.getName() == "" || conf.getHost()=="" 
			|| conf.getPort() < 1024 || conf.getPort() > 65535 || !conf.getBacklog())
				return false;
			return true;
		}

		//init
		int initServer()
		{
			if (!isValidConf())
			{	
				std::cout << "Invalid configuration " << getConfig() << std::endl;
				return (-1);
			}
			// Create server socket
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sock <= 0)
			{
				std::cout << "Error creating socket" << std::endl;
				return (-2);
			}
			// Set socket options
			int sockopt_val = 1;
			if ( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt_val, sizeof(sockopt_val)) )
			{
				perror("setsockopt");
				std::cout << "Error setting socket options" << std::endl;
				return (-3);
			}
			address.sin_family = AF_INET;
			address.sin_addr.s_addr =  INADDR_ANY;
			address.sin_port = htons(conf.getPort());

			if (bind(sock, (struct sockaddr *)&address, sizeof(address)))
			{
				perror("bind");
				std::cout << "Error bind socket to port" << std::endl;
			}
			if(listen(sock, 3)){
				perror("listen");
				std::cout << "Error listening" << std::endl;
			}
			std::cout << "Listening on port " << conf.getPort() << std::endl;
			return (0);
		}
	
	int run (){

		int tmp = 0;
		while (1)
		{
			int new_sock;
			int address_size = sizeof(address);

			std::cout << "waiting for connection" << conf.getPort() << std::endl;
			if((new_sock = accept(sock, (struct sockaddr *)&address, (socklen_t*)&address_size)) < 0)
			{
				perror("accept");
				std::cout << "Error accepting" << std::endl;
				return (-1);
			}
			std::cout << "accepted connection on port " << conf.getPort() << std::endl;
			char buffer[102400] = {0};
			if(tmp!=new_sock)
			{
				fcntl(new_sock, F_SETFL,O_NONBLOCK);
				tmp = new_sock;
			}
			int rd = 0;
			while(rd == 0) 
				rd = recv( new_sock , buffer, 102400, 0);
			Request req(buffer);
			
			std::string response("HTTP/1.1 200 OK\r\nAA:OO\r\nBB:OO\r\nCC:OO\r\n\r\nWAAAAAAAAAA\r\n");
			send(new_sock, response.c_str(), response.size(), 0);
			close(new_sock);
		}
		return (0);
	}

};
//fcntl(fd, F_SETFL, O_NONBLOCK);
std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */