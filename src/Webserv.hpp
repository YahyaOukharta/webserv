#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>

# include "Server.hpp"
# include "Parser.hpp"

class Webserv
{
	std::vector<Server*> servers;

	public:

		Webserv(){
		}

		Webserv(std::string const & config_file){
			std::vector<ServerConfig> configs = Parser::parseFile(config_file);
			for(std::vector<ServerConfig>::iterator it = configs.begin(); it != configs.end();++it){
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
			// for (std::vector<Server*>::iterator it = servers.begin(); it != servers.end();++it){
			// 	delete (*it);
			// }
		}

		Webserv &		operator=( Webserv const & rhs ){
			servers = rhs.servers;
			return (*this);
		}

		std::vector<Server*> const&getServers()const{
			return servers;
		}

		void run(){
			while(1){
				for (std::vector<Server*>::iterator it = servers.begin(); it != servers.end();++it){
					std::cout << (*it)->async() << std::endl;
				}
			}
		}

	private:

};

std::ostream &			operator<<( std::ostream & o, Webserv const & i );

#endif /* ********************************************************* WEBSERV_H */