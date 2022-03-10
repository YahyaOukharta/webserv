#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include "ServerConfig.hpp"
# include "Location.hpp"
# include "Utils.hpp"
#include <fstream>
#include <istream>

class Parser
{
	private:

	public:

		typedef std::vector<std::string> str_vec;
		typedef std::map<std::string,std::string> str_map;

		typedef std::map<std::string, str_map> servers_fields_map;
		typedef std::map<std::string, str_vec > servers_loc_map;

		Parser();
		Parser( Parser const & src );
		~Parser();

		Parser &		operator=( Parser const & rhs );

		static std::vector<ServerConfig> parseFile(std::string conf_path)
		{
			std::ifstream input( conf_path );
			if (!input.is_open())
			{
				// std::cout << "Error opening file "<< conf_path << std::endl;
				throw webserv_exception("Invalid config file path "+conf_path);
			}

			servers_fields_map servers_fields; // field like port host ..
			servers_loc_map servers_locations; // locations

			std::string current_srv_name;

			int line_counter = 0;
			for( std::string line; std::getline( input, line ); )
			{
				line_counter++;
				trim(line);
				if (line == "" || line[0]=='#')
					continue;
				int i = 0;
				if ((i = line.find("#"))>0)
					line = line.substr(0, i);

				// server name
				if (line[0] == '[' && (i = line.find("]",1)) > 0 )
				{
					current_srv_name = line.substr(1,i - 1);
					if (line.substr(i+1) != "")
					{
						// std::cout << "Error near server name  " << std::endl;
						throw webserv_exception("Error near server name");
					}
					if (current_srv_name != "")
					{
						//std::cout << "Server name = <" << current_srv_name <<">"<< std::endl;
						continue;
					}
					else{
						// std::cout << "Error with server name  " << std::endl;
						throw webserv_exception("Invalid server name 1");
					}
				}
				else if (line[0] == '[')
				{
					// std::cout << "Error with server name  " << std::endl;
					throw webserv_exception("Invalid server name 2");
				}
				// other fields
				if ((i = line.find("=",1)) > 0)
				{
					std::string field = line.substr(0,i);
					trim(field);
					std::string value = line.substr(i+1);
					trim(value);
					if (value[value.size() - 1] != ';')
					{
						// std::cout << "Missing semicolon at the end of line  "<<line_counter << std::endl;
						throw webserv_exception("missing semicolon near line " + std::to_string(line_counter));
					}
					//std::cout << "key=<"<<field<<"> "<< "value=<"<<value<<"> " << std::endl;
					if (field =="location")
						servers_locations[current_srv_name].push_back(value.substr(0, value.size()-1));
					else
						servers_fields[current_srv_name].insert(std::make_pair(field, value.substr(0, value.size() - 1)));
				}
				else
				{
					// std::cout << "Error with field in line " << line_counter << std::endl;
					throw webserv_exception("Error with field in line " + std::to_string(line_counter));
				}
			}

			std::vector<ServerConfig> configs;
			for (servers_fields_map::iterator it = servers_fields.begin(); it != servers_fields.end(); ++it){
				std::cout << "[" << it->first << "]" << std::endl;
				ServerConfig cfg = parseServer(it->second, it->first, line_counter);

				std::cout << " Locations : " << std::endl;
				std::vector<Location> locs;
				for (str_vec::iterator it2 = servers_locations[it->first].begin(); it2 != servers_locations[it->first].end(); ++it2)
				{
					//std::cout << "  "<< *it2 << std::endl;
					locs.push_back(parseLocation(*it2, line_counter));
				}
				cfg.setLocations(locs);
				configs.push_back(cfg);
				std::cout << std::endl;
			}

			return configs;
		}

		static ServerConfig parseServer(str_map server_fields, std::string server_name, int line_counter) {

			std::string host ;  // server host, ie 0.0.0.0
			int			port;  // server port, ie 8080

			int 		body_size_limit; // body size limit 
			std::string default_error_page; // error page
			std::vector<std::string> allowed_methods; // GET POST ..

			int backlog = 30;

			for (str_map::iterator it = server_fields.begin(); it != server_fields.end(); ++it)
			{
				std::cout << " "<< it->first << "=" << it->second << std::endl;
				if (it->first=="host")
					host = it->second;
				else if (it->first=="port")
					port = std::stoi(it->second);
				else if (it->first=="body_size_limit")
					body_size_limit = std::stoi(it->second);
				else if (it->first=="allowed_methods")
					allowed_methods = split_to_lines(it->second,"/");
				else if (it->first=="default_error_page")
					default_error_page = it->second;
				else if (it->first =="body_size_limit")
					body_size_limit = std::stoi(it->second);
				else
				{
					//std::cout << "Error with Server config, unknown field <"<< it->first << "> in line " << line_counter << std::endl;
					throw webserv_exception("Error with Server config, unknown field <"+ it->first +"> in line " + std::to_string(line_counter));
				}
			}
			return ServerConfig(server_name,host,port,body_size_limit,default_error_page,allowed_methods, backlog);
		}
		static Location parseLocation(std::string loc, int line_counter) {

			std::string path;
			std::string root;
			str_vec allowed_methods;
			int			body_size_limit;
			int			auto_index;
			std::string error_page;

			if (loc[0] !='{' || loc[loc.size()-1] !='}')
			{
				//std::cout << "Error with Location in line " << line_counter << std::endl;
				throw webserv_exception("Error with Location in line " + std::to_string(line_counter));
			}
			loc = loc.substr(1,loc.size()-2);
			trim(loc);
			std::cout << "loc to parse :" << loc << std::endl;
			str_vec sp_loc = split_to_lines(loc,",");
			for (str_vec::iterator it = sp_loc.begin(); it != sp_loc.end();++it)
			{
				str_vec sp = split_to_lines(*it, "=");
				if (sp.size() != 2)
				{
					//std::cout << "Error with Location in line " << line_counter << std::endl;
					throw webserv_exception("Error with Location in line " + std::to_string(line_counter));
				}
				std::string key = sp[0];
				std::string val = sp[1];
				trim(key);
				trim(val);
				//std::cout << key << "=" << val << std::endl;

				if (key=="path")
					path = val;
				else if (key=="root")
					root = val;
				else if (key=="autoindex")
					auto_index = val == "on" ? 1 : 0;
				else if (key=="methods")
					allowed_methods = split_to_lines(val,"/");
				else if (key=="error_page")
					error_page = val;
				else if (key =="body_size_limit")
					body_size_limit = std::stoi(val);
				else
				{
					//std::cout << "Error with Location, unknown field <"<< key << "> in line " << line_counter << std::endl;
					throw webserv_exception("Error with Location, unknown field <"+ key +"> in line " + std::to_string(line_counter));
				}
			}
			return Location(path, root, allowed_methods, body_size_limit, auto_index, error_page);
		}
};

std::ostream &			operator<<( std::ostream & o, Parser const & i );




#endif /* ********************************************************** PARSER_H */