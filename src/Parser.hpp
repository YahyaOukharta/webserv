#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include "ServerConfig.hpp"
# include "Location.hpp"

#include <fstream>
#include <istream>

class Parser
{
	private:

	public:
		typedef std::map<std::string,std::string> str_map;
		typedef std::map<std::string, str_map> servers_fields_map;
		typedef std::map<std::string, std::vector<std::string> > servers_loc_map;

		Parser();
		Parser( Parser const & src );
		~Parser();

		Parser &		operator=( Parser const & rhs );

		static ServerConfig parseFile(std::string conf_path)
		{
			std::ifstream input( conf_path );
			if (!input.is_open())
			{
				std::cout << "Error opening file "<< conf_path << std::endl;
				throw webserv_exception("Invalid config file path");
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
						std::cout << "Error near server name  " << std::endl;
						throw webserv_exception("Error near server name");
					}
					if (current_srv_name != "")
					{
						//std::cout << "Server name = <" << current_srv_name <<">"<< std::endl;
						continue;
					}
					else{
						std::cout << "Error with server name  " << std::endl;
						throw webserv_exception("Invalid server name 1");
					}
				}
				else if (line[0] == '[')
				{
					std::cout << "Error with server name  " << std::endl;
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
						std::cout << "Missing semicolon at the end of line  "<<line_counter << std::endl;
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
					std::cout << "Error with field in line " << line_counter << std::endl;
					throw webserv_exception("Error with field in line " + std::to_string(line_counter));
				}
			}

			for (servers_fields_map::iterator it = servers_fields.begin(); it != servers_fields.end(); ++it){
				std::cout << "[" << it->first << "]" << std::endl;
				for (str_map::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
				{
					std::cout << " "<< it2->first << "=" << it2->second << std::endl;
				}
				std::cout << " Locations : " << std::endl;
				for (std::vector<std::string>::iterator it2 = servers_locations[it->first].begin(); it2 != servers_locations[it->first].end(); ++it2)
				{
					std::cout << "  "<< *it2 << std::endl;
				}
				std::cout << std::endl;
			}	

			return ServerConfig();
		}


		Location parseLocation() {
			return Location();
		}
};

std::ostream &			operator<<( std::ostream & o, Parser const & i );




#endif /* ********************************************************** PARSER_H */