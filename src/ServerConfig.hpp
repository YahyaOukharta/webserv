#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <string>

# include "./Location.hpp"
#include <vector>
class ServerConfig
{

	private:
		std::string _name; // server name
		std::string _host ;  // server host, ie 0.0.0.0
		int			_port;  // server port, ie 8080
		//int 		_body_size_limit; // body size limit 
		std::string _default_error_page; 

		std::vector<Location> _locations;
		std::vector<std::string> _allowed_methods; // GET POST ..

		int			_backlog; //request queue size



	public:
		ServerConfig(){};

		ServerConfig(std::string name, std::string host, int port, int backlog){
			_name = name;
			_host = host;
			_port = port;
			_backlog = backlog;
		}

		ServerConfig( ServerConfig const & src ){
			*this = src;
		}

		~ServerConfig(){}

		ServerConfig &		operator=( ServerConfig const & rhs ){
			_name = rhs.getName();
			_host = rhs.getHost();
			_port = rhs.getPort();
			_backlog = rhs.getBacklog();
			return *this;
		}

		std::string getName() const{
			return _name;
		}
		std::string getHost() const{
			return _host;
		}
		int			getPort() const{
			return _port;
		}
		int			getBacklog() const{
			return _backlog;
		}


};

std::ostream &			operator<<( std::ostream & o, ServerConfig const & i ){
	o << "Server | Name : <" << i.getName();
	o << "> Host : <" << i.getHost();
	o << "> Port : <" << i.getPort();
	o << "> Backlog : <" << i.getBacklog() << ">";
	return o;
}

#endif /* **************************************************** SERVERCONFIG_H */