#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <string>

# include "./Location.hpp"
#include <vector>

#include "Config.hpp"

#include "FileSystem.hpp"
#include "Utils.hpp"
class ServerConfig
{

	private:
		std::string _name; // server name

		std::string _host ;  // server host, ie 0.0.0.0
		int			_port;  // server port, ie 8080

		int 		_body_size_limit; // body size limit 
		std::string _default_error_page; // error page
		std::vector<std::string> _allowed_methods; // GET POST ..
		std::vector<std::string> _index; // GET POST ..


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
			std::vector<std::string> index,

			int backlog
		){
			_name = name;
			_host = host;
			_port = port;
			_backlog = backlog;

			_body_size_limit = body_size_limit;
			_allowed_methods = allowed_methods;
			_default_error_page = default_error_page;
			_index = index;
		};
		void setLocations(std::vector<Location> const &locs){
			_locations = locs;
		}
		void setLocations(std::vector<Config::Location> const &locs){
			_locations.clear();
			for (std::vector<Config::Location>::const_iterator it = locs.begin(); it != locs.end(); ++it)
			{
				Location loc(
					it->path,
					it->root,
					it->allowed_methods,
					it->bodysize_limit,
					it->autoindex == "on" ? 1 : 0, /// ???? AUTO INDEX SHOULD BE BOOL OR INT
					it->default_error_pages,
					ft::split_to_lines(it->index, "/"),
					it->cgi_path,
					it->extension,
					it->upload_path,
					it->redirect
				);
				
				if(loc.getErrorPage() != "" && !FileSystem::fileExists(loc.getErrorPage()))
					throw webserv_exception(std::string("Invalid path for location error_page ") + loc.getErrorPage());
				_locations.push_back(loc);
			}
			
			std::sort(_locations.begin(), _locations.end(), Location::greater_than_path());
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
			_index = rhs.getIndex();
			setLocations(rhs.getLocations());
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
		std::vector<std::string> const &getIndex() const{
			return _index;
		}
		std::string const &getDefaultErrorPage() const {
			return _default_error_page;
		}

		std::vector<Location> const &getLocations() const{
			return _locations;
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
