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

		int 		_body_size_limit; // body size limit 
		std::string _default_error_page; // error page
		std::vector<std::string> _allowed_methods; // GET POST ..

		std::vector<Location> _locations;

		int			_backlog; //request queue size



	public:
		ServerConfig(){}
		
		ServerConfig(
			std::string name,

			std::string host,
			int			port,

			int 		body_size_limit,
			std::string default_error_page,
			std::vector<std::string> allowed_methods,

			int backlog
		){
			_name = name;
			_host = host;
			_port = port;
			_backlog = backlog;

			_body_size_limit = body_size_limit;
			_allowed_methods = allowed_methods;
			_default_error_page = default_error_page;
		};
		void setLocations(std::vector<Location> const &locs){
			_locations = locs;
		}

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

		ServerConfig &		operator=( ServerConfig const & rhs )
		{
			_name = rhs.getName();
			_host = rhs.getHost();
			_port = rhs.getPort();
			_backlog = rhs.getBacklog();

			_body_size_limit = rhs.getBodySizeLimit();
			_allowed_methods = rhs.getAllowedMethods();
			_default_error_page = rhs.getDefaultErrorPage();

			return *this;
		}

		std::string const &getName() const{
			return _name;
		}
		std::string const &getHost() const{
			return _host;
		}
		int			getPort() const{
			return _port;
		}
		int			getBacklog() const{
			return _backlog;
		}

		int getBodySizeLimit() const {
			return _body_size_limit;
		}
		std::vector<std::string> const &getAllowedMethods() const {
			return _allowed_methods;
		}
		std::string const &getDefaultErrorPage() const {
			return _default_error_page;
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
