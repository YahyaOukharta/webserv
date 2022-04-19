#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>

# include "Server.hpp"
// # include "Parser.hpp"
# include "Request.hpp"
# include "Response.hpp"
#include "FileSystem.hpp"

class Webserv
{
	public:
		typedef std::vector<Server*> srv_vec;
		typedef std::vector<Server*>::iterator srv_vec_it;
		typedef std::vector<Server*>::const_iterator srv_vec_cit;
	private:

	std::vector<Server*> servers;
	std::map<int, int> client_to_srv_idx;

	std::map<int, std::string> client_to_buf; 
	std::map<int, Request> client_to_req; 

	std::map<int, std::string> client_to_res_buf;

	public:

		Webserv(){
		}

		// Webserv(std::string const & config_file){
		// 	std::vector<ServerConfig> configs = Parser::parseFile(config_file);
		// 	for(std::vector<ServerConfig>::iterator it = configs.begin(); it != configs.end();++it){
		// 		Server *srv = new Server(*it);
		// 		srv->initServer();
		// 		servers.push_back(srv);
		// 	}
		// }
		Webserv(std::string const & config_file){
			Config cfg(config_file);
			for(std::vector<Config::Server>::iterator it = cfg._servers.begin(); it != cfg._servers.end(); ++it){
				Server *srv = new Server(*it);
				srv->initServer();
				servers.push_back(srv);
			}
		}

		Webserv( Webserv const & src )
		{
			*this = src;
		}
		~Webserv(){
			// for (srv_vec_cit it = servers.begin(); it!= servers.end(); ++it){
			// 	delete(*it);
			// }

		}

		Webserv &		operator=( Webserv const & rhs ){
			servers = rhs.servers;
			return (*this);
		}

		std::vector<Server*> const&getServers()const{
			return servers;
		}

		int getServerIndexWithSock(int sock) const{
			for (srv_vec_cit it = servers.begin(); it!= servers.end(); ++it){
				if ((*it)->getSocket() == sock)
					return it - servers.begin();
			}
			return (-1);
		}
		void run(){
			// while (1){
			// 	for (std::vector<Server*>::iterator it = servers.begin(); it != servers.end();++it){
			// 		std::cout << (*it)->async() << std::endl;
			// 	}
			// }
			fd_set master_rd_set, working_rd_set; // reading fd sets
			fd_set master_wr_set, working_wr_set; // writing fd sets
			struct timeval select_timeout = {60, 0}; 
			int max_fd = 0;

			FD_ZERO(&master_rd_set);
			FD_ZERO(&master_wr_set);
			
			for (srv_vec_it it = servers.begin(); it!= servers.end(); ++it){ //add servers socks to reading set
				int sock = (*it)->getSocket();
				FD_SET (sock, &master_rd_set);
				if (sock > max_fd)
					max_fd = sock;
			}

			int select_ret;
			while (1){
				memcpy(&working_rd_set, &master_rd_set, sizeof(master_rd_set));
				memcpy(&working_wr_set, &master_wr_set, sizeof(master_wr_set));
				
				// std::cout << "sleeping 2 sec " << std::endl;
				// usleep(1e5);

				select_ret = select(max_fd + 1, &working_rd_set, &working_wr_set,NULL, &select_timeout);
				if (select_ret == -1)
				{
					perror("ss");
					std::cout << "select failed" << std::endl;
					return;
				}
				if (select_ret == 0)
				{
					std::cout << "select timed out, pending reqs="<<client_to_req.size() << std::endl;
					return;
				}
				for (int fd = 0; fd <= max_fd && select_ret > 0; ++fd){
					if (FD_ISSET(fd, &working_rd_set)){ // fd is ready for writing
						select_ret--;
						int srv_index;
						if ((srv_index = getServerIndexWithSock(fd)) != -1){ // server socket ready for reading
							// accept all connections
							int client_sock = 0;
							while (client_sock != -1){
								client_sock = servers[srv_index]->accept_connection();
								if (client_sock == -1){
									if (errno != EWOULDBLOCK)
									{
										std::cout << "accepting failed" << std::endl;
										return;
									}
									break;
								}

								client_to_srv_idx[client_sock] = srv_index; // map client sock to server it came from 
								FD_SET(client_sock, &master_rd_set); // add client sock to master reading set

								if (client_sock > max_fd) max_fd = client_sock; // update max_fd
							}
						}
						else { // client socket ready for reading
							char buff[1024 + 1];
							int rd = recv(fd, buff, 1024, 0);
							if (rd == -1 ){ // recv failed
								perror("recv :");
								close(fd);
								FD_CLR(fd, &master_rd_set);
								while (FD_ISSET(max_fd, &master_rd_set) == 0)
									max_fd -= 1;
							}
							client_to_buf[fd].append(buff, rd);
							if (rd == 0 || (rd = recv(fd, buff, 1024, 0)) <= 0)
							{
								std::cout << "in req\n";
								// client_to_buf[fd].append(buff, rd);
								// try{
								// 	Request req(client_to_buf[fd]);
								// 	if(req.getVersion().size()) rd = 0;
								// }
								// catch(webserv_exception const &e){
								// 	//std::cout << e.what() << std::endl;
								// 	(void)e;
								// }
								// if (rd == 0)
								// { // done reading
									try{
										std::cout << "[" << client_to_srv_idx[fd] << "] " ;
										Request req(client_to_buf[fd]);
										req.print();
										client_to_req[fd] = req;
									}
									catch(webserv_exception const& e){ // bad request
										std::cout << e.what() << std::endl;
										client_to_buf.erase(fd);
									}
									FD_CLR(fd, &master_rd_set);
									FD_SET(fd, &master_wr_set);
									client_to_buf.erase(fd);
								// }
							}
							else
							{
								std::cout << "APPENDING\n";
								client_to_buf[fd].append(buff, rd);
							}
						}
					}
					else if(FD_ISSET(fd, &working_wr_set)){
						if (client_to_req[fd].getVersion() != "")
						{
							Response res(client_to_req[fd], servers[client_to_srv_idx[fd]]);
							client_to_res_buf[fd].append(res.getResponseBufferWithoutBody());
							client_to_res_buf[fd].append(FileSystem::getFileContent(res.getRessourcePath())+"\r\n");
							
							client_to_req.erase(fd);
						}
						ssize_t ret = send(fd, client_to_res_buf[fd].c_str(), client_to_res_buf[fd].size(), 0);
						client_to_res_buf[fd].erase(client_to_res_buf[fd].begin(), client_to_res_buf[fd].begin() + ret);

						if (ret == -1 || !client_to_res_buf[fd].size())
						{
							client_to_res_buf.erase(fd);
							close(fd);
							FD_CLR(fd, &master_wr_set);
							while(FD_ISSET(max_fd, &master_rd_set) == 0 && FD_ISSET(max_fd, &master_wr_set) == 0)
								max_fd--;
						}
					}
				}
			}
			for (int i = 0; i < max_fd; i++){
				if(FD_ISSET(i, &master_rd_set)){
					close(i);
				}
			}
		}

	private:

};

std::ostream &			operator<<( std::ostream & o, Webserv const & i );

#endif /* ********************************************************* WEBSERV_H */

