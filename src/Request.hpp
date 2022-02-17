#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
#include "Utils.hpp"

class Request
{
	private:
		std::string protocol; // HTTP
		std::string version;  // 1.1

		std::string path;  //   /ee/aa/
		std::string query; //	?hello=1&dkd=22
		
		std::map<std::string, std::string> headers;

		std::string body;

	public:

		Request(){

		}
		Request(std::string raw_req){
			parse_request(raw_req);
		}
		Request( Request const & src );
		~Request(){}

		Request &		operator=( Request const & rhs );

		int parse_request(std::string raw_req){
			std::vector<std::string> lines = split_to_lines(raw_req,"\r\n\r\n\r\n");
			if(lines.size()==2)
				body = lines[1];
			
			
			return (0);
		}
	private:

};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */