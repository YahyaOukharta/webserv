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
#include "FileSystem.hpp"

#include <errno.h>
#include <cstring>

#include "Config.hpp"
    // #include <arpa/inet.h> /// inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN); /// from sockaddr_in to string 


class Server
{
	private:
		ServerConfig conf;
		int sock;
		struct sockaddr_in address;
		struct sockaddr_in client_address;
		int address_size;


	public:
		Server(){

		};

		Server(ServerConfig c){
			conf = c;
		};
		Server(std::string name, std::string host, int port, int backlog){
			conf = ServerConfig(name, host, port, backlog);
		};
		Server(Config::Server srv_conf){ // hakim
			conf = ServerConfig(
				srv_conf.name,

				srv_conf.host,
				srv_conf.port,

				srv_conf.bodysize_limit,
				srv_conf.default_error_pages,
				srv_conf.allowed_methods,
				ft::split_to_lines(srv_conf.index, "/"),
				1000
			);
			if(!FileSystem::fileExists(conf.getDefaultErrorPage()))
				throw webserv_exception(std::string("Invalid path for error_page ") + conf.getDefaultErrorPage());
			conf.setLocations(srv_conf.locations);
		};
		Server( Server const & src ){
			*this = src;
		}
		~Server(){
			//close(sock);
		}

		Server &		operator=( Server const & rhs ){
			conf = rhs.getConfig();
			return *this;
		}

		//Config
		ServerConfig const &getConfig()const {
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


		// Socket init
		int init_socket(){
			// Create server socket
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sock <= 0)
			{
				throw(webserv_exception("Error creating socket"));
				//return (-2);
			}

			// Set socket options
			int sockopt_val = 1;
			if ( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt_val, sizeof(sockopt_val)) )
			{
				//perror("setsockopt");
				throw(webserv_exception("Error setting socket options"));

				return (-3);
			}
			address.sin_family = AF_INET;
			address.sin_addr.s_addr =  INADDR_ANY;
			address.sin_port = htons(conf.getPort());
			return (0);
		}

		int bind_socket()
		{
			if (bind(sock, (struct sockaddr *)&address, sizeof(address)))
			{
				//perror("bind");
				throw(webserv_exception("Error bind socket to port"));

			}
			return (0);
		}

		// init
		int initServer()
		{
			address_size = sizeof(address);
			if (!isValidConf()) // add more validation for new variables (max body size, etc ..)
			{	
				std::cout << "Invalid configuration " << getConfig() << std::endl;
				throw(webserv_exception("Error in Config"));

				return (-1);
			}

			//init socket
			int error;
			if((error = init_socket())){
				throw(webserv_exception("Error in init_socket : " + std::to_string(error)));
				return (error);
			}

			//bind to address
			if((bind_socket())){
				//perror("listen");
				throw(webserv_exception("Error binding "));
				return (-3);
			}

			// listen
			if(listen(sock, 3)){
				//perror("listen");
				throw(webserv_exception("Error listening "));
				return (-4);
			}

			// set server sock to non blocking mode
			fcntl(sock, F_SETFL, O_NONBLOCK);

			 std::cout << "Server ready on port " << conf.getPort() << std::endl;
			return (0);
		}

		int accept_connection()
		{
			int client_sock;
			if((client_sock = accept(sock, (struct sockaddr *)&client_address, (socklen_t*)&address_size)) < 0)
			{
				//std::cout << "Error Accepting" << std::endl;
				return (-1);
			}
			fcntl(client_sock, F_SETFL, O_NONBLOCK);

			// display client ip
			// char str[20]={0};
			// inet_ntop(AF_INET, &(client_address.sin_addr), str, 20);
			// std::cout<<str<< std::endl;
			return client_sock;
		}

//int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout);
		int async(){
			fd_set master_set, working_set; 
			struct timeval timeout = {1,0};
			int max_fd = sock;

			FD_ZERO(&master_set);
			FD_SET(sock, &master_set);

			int ret;
			while (1){
				memcpy( &working_set,&master_set,sizeof(master_set));
				// std::cout <<"sleeping" << std::endl;
				// usleep(5000000);
				ret = select(max_fd + 1, &working_set, NULL,NULL,&timeout);
				if(ret == -1)
				{
					std::cout << "select failed" << std::endl;
					return (1);
				}
				if(ret == 0)
				{
					std::cout << "select timed out" << std::endl;
					return (1);
				}

				// std::cout << "select ret="<<ret << " max_fd="<<max_fd <<" sock="<<sock << std::endl;
				for (int fd = 0; fd <= max_fd && ret > 0; fd++){
					if(FD_ISSET(fd,&working_set)){
						ret--;						
						if (fd == sock){ // server ready
							//std::cout << "server socket is ready" << std::endl;
							int client_sock = 0;

							while (client_sock != -1) { // Add to queue all incoming connections
								client_sock = accept_connection();
								if(client_sock == -1){
									if(errno != EWOULDBLOCK){
										std::cout << "Accept failed errno!=EWOUlDBLOCK" << std::endl;
										return (1);
									}
									break;
								}
								//std::cout << "new connection on sock " << client_sock << std::endl;
								FD_SET(client_sock, &master_set);
								if (client_sock > max_fd)
									max_fd = client_sock;
							}
						}
						else { // client ready
							std::string buf = FileSystem::getFileContent(fd);
							try
							{
								Request req(buf);

								std::string filepath = req.getPath() == "/" ? req.getPath()+"index.html" : req.getPath();
								std::string response("HTTP/1.1 200 OK\r\nAA:OO\r\nBB:OO\r\nCC:OO\r\n\r\n");

								response.append(FileSystem::getFileContent("www"+filepath)+"\r\n");

								send(fd, response.c_str(), response.size(), 0);

							}
							catch(const webserv_exception& e)
							{
								std::string response("HTTP/1.1 500 ERROR\r\n\r\nOops\r\n");
								send(fd, response.c_str(), response.size(), 0);
								std::cerr << "Error : "<< e.what() << '\n';
							}
							close(fd);
							FD_CLR(fd, &master_set);
							if (fd == max_fd){
								while(FD_ISSET(max_fd, &master_set) == 0 )
									max_fd--;
							}
						}
					}
				}
			}
			for (int i = 0; i < max_fd; i++){
				if(FD_ISSET(i, &master_set)){
					close(i);
				}
			}
		}

		int run (){
			// no select
			int tmp = 0;
			while (1)
			{
				int new_sock;
				std::cout << std::endl << "waiting for connection ..." << std::endl << std::endl;
				if((new_sock = accept_connection()) < 0)
				{
					//perror("accept");
					std::cout << "Error accepting" << std::endl;
					return (-1);
				}
				std::cout << "-> Request :" << conf.getPort() << std::endl;
				char buffer[102400] = {0};
				if(tmp!=new_sock)
				{
					fcntl(new_sock, F_SETFL,O_NONBLOCK);
					tmp = new_sock;
				}

				recv( new_sock , buffer, 102400, 0);
				// std::cout << "buffer <"<<buffer<<">" <<std::endl<<std::endl;
				try
				{
					Request req(buffer);
					std::string response("HTTP/1.1 200 OK\r\nAA:OO\r\nBB:OO\r\nCC:OO\r\n\r\nWAAAAAAAAAA\r\n");
					send(new_sock, response.c_str(), response.size(), 0);
					close(new_sock);
				}
				catch(const webserv_exception& e)
				{
					std::string response("HTTP/1.1 500 ERROR\r\n\r\nOops\r\n");
					send(new_sock, response.c_str(), response.size(), 0);
					close(new_sock);
					std::cerr << "Error : "<< e.what() << '\n';
				}
			}
			return (0);
		}

		int getSocket() const {
			return sock;
		}

		int getMatchingLocationIndex(Request &req){
			// 
			//  /  /images/  /images/hd/image.pmg
			// /
			std::vector<Location> const &locs = conf.getLocations();
			std::string path = req.getPath();
			
			size_t last_slash = path.rfind('/');
			if(last_slash != path.size()-1)
			for (std::vector<Location>::const_iterator it = locs.begin(); it != locs.end(); ++it)
			{
				if (it->getPath() == path)
				{
					path+='/';
					req.addSlashToPath();
					break;
				}
			}
			last_slash = path.rfind('/');
			std::string route = path.substr(0,(last_slash != std::string::npos ? last_slash + 1 : std::string::npos));

			std::cout << "Requested path : " << path << " - route : " <<route <<  std::endl;

			for (std::vector<Location>::const_iterator it = locs.begin(); it != locs.end(); ++it)
			{
				std::string loc_path = it->getPath();
				if (*(--loc_path.end()) != '/') loc_path.push_back('/');
				if (route.find(loc_path) == 0)
				{
					return it - locs.begin();
				}
			}
			return -1;
		}
};

//fcntl(fd, F_SETFL, O_NONBLOCK);
std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
