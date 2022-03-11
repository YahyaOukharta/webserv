#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
#include "Utils.hpp"

# define DEBUG 0
#include "Response.hpp"
class Request
{

	public:
		typedef std::vector<std::string> vec;
		typedef std::vector<std::string>::iterator iter;

	private:
		std::string protocol; // HTTP
		std::string version;  // 1.1

		std::string method; // GET POST OPTIONS ...
		std::string path;  //   /ee/aa/
		std::string query; //	?hello=1&dkd=22
		
		std::map<std::string, std::string> headers;



		std::string body;


	public:

		Request(){

		}
		Request(std::string raw_req)
		{
			int error;
			if ((error = parse_request(raw_req))){
				if (error == 1)
					throw webserv_exception("Bad request");
				if (error == 2)
					throw webserv_exception("Unsupported method");
				if (error == 3)
					throw webserv_exception("Too many '?'");
			}
			print();
		}
		Request( Request const & src ){
			*this = src;
		}
		~Request(){}

		Request &		operator=( Request const & rhs )
		{
			protocol = rhs.getProtocol();
			version = rhs.getVersion();			
			method = rhs.getMethod();
			path = rhs.getPath();
			query = rhs.getQuery();
			headers = rhs.getHeaders();
			body = rhs.getBody();
			return *this;
		}

		// parsing

		int parse_request(std::string raw_req){

			vec req_split_body = split_to_lines(raw_req,"\r\n\r\n");
			if (req_split_body.size() != 2)
				return (1);
			body = req_split_body[1];


			//including first line
			vec head = split_to_lines(req_split_body[0]);

			//parse first line
			std::string first_line = head[0]; // line 1
			int ret = parse_first_line(first_line);
			if (ret) return ret;

			//parse headers
			head.erase(head.begin());
			for (iter it = head.begin(); it != head.end(); ++it)
			{
				vec header = split_to_lines(*it, ":");
				headers[header[0]] = header[1];
			}
			return (0);
		}

		int parse_first_line(std::string first_line)
		{
			vec tokens = split_to_lines(first_line," ");
			if (tokens.size() != 3)
				return 1;
			method = tokens[0];
			vec tmp = split_to_lines(tokens[1],"?");
			if (tmp.size() > 2)
				return (3);
			path = tmp[0];
			query= tmp.size()==2 ? tmp[1] : "";
			tmp = split_to_lines(tokens[2],"/");
			if (tmp.size() != 2)
				return (1);
			protocol = tmp[0];
			version = tmp[1];
			return (0);
		}
		
		int validate_request(){
			return 0;
		}

		// getters
		const std::string &getProtocol() const {
			return protocol;
		}
		const std::string &getVersion() const {
			return version;
		}

		const std::string &getMethod() const {
			return method;
		}
		const std::string &getPath() const {
			return path;
		}
		const std::string &getQuery() const {
			return query;
		}
		const std::map<std::string, std::string> &getHeaders() const {
			return headers;
		}
		const std::string getHeader(std::string const &key) {
			return headers[key];
		}
		const std::string &getBody() const {
			return body;
		}

		// debug
		void print() {
			std::cout 
				<< method << " " 
				<< path << " " 
				<< query << " " 
				<< protocol << "/" 
				<< version <<"  ";
			if (DEBUG)
			{
				std::cout << "\nHeaders:" << std::endl;
				for(std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); ++it){
					std::cout << " " << it->first << " " << it->second << std::endl;
				}
				std::cout << "\nBody:\n" <<body<< std::endl;
				Response a;
				return;
			}
			std::cout << headers.size() << " headers   ";
			if (body.size())
				std::cout << "Body size : " << body.size() << " bytes"<< std::endl;
			else
				std::cout << "Empty body" << std::endl;

		}

	private:

};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */
