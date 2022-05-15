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

	std::map<int, std::string> client_to_req_buf; 
	std::map<int, Request> client_to_req; 

	std::map<int, std::string> client_to_res_buf;
	std::map<int, Response> client_to_res;

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
			struct timeval select_timeout = {1,0}; 
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
					//std::cout << "select timed out, pending reqs="<<client_to_req.size() << std::endl;
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
									break;
								}

								client_to_srv_idx[client_sock] = srv_index; // map client sock to server it came from 
								FD_SET(client_sock, &master_rd_set); // add client sock to master reading set

								if (client_sock > max_fd) max_fd = client_sock; // update max_fd
							}
						}
						else { // client socket ready for reading

							// std::string buf; // = FileSystem::getFileContent(fd);
							char buff[2000000] = {0};
							int rd = recv(fd, buff, 2000000, 0);
							// std::cout << "RD = " << rd << std::endl;
							// std::cout << "BUFF = \n" << buff << std::endl;
							if (rd == -1 ){ // recv failed
								perror("recv :");
								close(fd);
								FD_CLR(fd, &master_rd_set);
								while (FD_ISSET(max_fd, &master_rd_set) == 0)
									max_fd -= 1;
							}
							else{
 								client_to_req_buf[fd].append(buff, rd);
								try{
									client_to_req[fd] = Request(client_to_req_buf[fd]);
									if(client_to_req[fd].getVersion().size()) rd = 0;
								}
								catch(webserv_exception const &e){
									//std::cout << e.what() << std::endl;
									(void)e;
								}
								if (rd == 0 )
								{ // done reading
									try{
										// std::cout << "\n[" << client_to_srv_idx[fd] << "] " ;
										if(!client_to_req[fd].getVersion().size())
										client_to_req[fd] = Request(client_to_req_buf[fd]);
										//req.print();

									}
									catch(webserv_exception const& e){ // bad request
										//std::cout << e.what() << std::endl;
										client_to_req_buf.erase(fd);
									}
									FD_CLR(fd, &master_rd_set);
									FD_SET(fd, &master_wr_set);
									//std::cout << "buf " << client_to_req_buf[fd] << std::endl;
									client_to_req_buf.erase(fd);
								}
							}
						}
					}
					else if(FD_ISSET(fd, &working_wr_set)){
						if (client_to_req[fd].getVersion() != "")
						{
							if (!client_to_res.count(fd))
								client_to_res.insert(
									std::pair<int, Response>(
										fd,
										Response(client_to_req[fd], servers[client_to_srv_idx[fd]])
									)
								);
							std::string buf = client_to_res[fd].getResponseBufferWithoutBody();
							if (buf == "")
							{
								size_t timeout_sec = 20; // timeout in seconds
								if (time(NULL) - client_to_req[fd].getTime() > timeout_sec)
									client_to_res[fd].timeout();
								continue;
							}

							std::cout << "[" << client_to_srv_idx[fd] << "] " << "\033[1;34m" << client_to_res[fd].getStatusCode() << "\033[0m " ;
							client_to_req[fd].print(); 

							client_to_res_buf[fd].append(buf);
							client_to_res_buf[fd].append(FileSystem::getFileContent(client_to_res[fd].getRessourcePath())+"\r\n");
							client_to_res.erase(fd);
							client_to_req.erase(fd);
						}
						ssize_t ret = send(fd, client_to_res_buf[fd].c_str(), client_to_res_buf[fd].size(), 0);
						//std::cout << "sent " << ret << " , total " << client_to_res_buf[fd].size() << " , " ;
						client_to_res_buf[fd].erase(client_to_res_buf[fd].begin(), client_to_res_buf[fd].begin() + ret);
						//std::cout << " left " << client_to_res_buf[fd].size() << std::endl ;

						if (ret == -1 || !client_to_res_buf[fd].size())
						{
							client_to_res_buf.erase(fd);
							close(fd);
							FD_CLR(fd, &master_wr_set);
							while(FD_ISSET(max_fd, &master_rd_set) == 0 && FD_ISSET(max_fd, &master_wr_set) == 0)
								max_fd--;
						}
						//

						// FD_CLR(fd, &master_wr_set);
						// while(FD_ISSET(max_fd, &master_rd_set) == 0 && FD_ISSET(max_fd, &master_wr_set) == 0)
						// 	max_fd--;
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

