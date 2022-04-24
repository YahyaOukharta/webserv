#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include <cstring>
#include "Utils.hpp"
# include <fcntl.h>
# define DEBUG 0
//# include "Response.hpp"

class Request
{

	public:
		typedef std::vector<std::string> vec;
		typedef std::vector<std::string>::iterator iter;

	private:
		std::string							protocol; // HTTP
		std::string							version;  // 1.1

		std::string							method; // GET POST OPTIONS ...
		std::string							path;  //   /ee/aa/
		std::string							query; //	?hello=1&dkd=22
		
		std::map<std::string, std::string>	headers; // split to general, request, and representation

		std::map<std::string, std::string>	representation_headers; // split to general, request, and representation
		std::map<std::string, std::string>	request_headers; // split to general, request, and representation

		std::string							boundary;

		std::string							body;
		std::string							body_filename;

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
				// if (error == 3)
				// 	throw webserv_exception("Too many '?'");

			}
			std::cout << "Boundary = " << boundary << std::endl;
			initRepresentationHeaders();
			initRequestHeaders();
			// if (method=="POST")
			// {
			// 	int t1 = time(NULL);
			// 	std::string fileName = "/tmp/body_" + std::to_string(t1);
			// 	int fd = open(fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
			// 	write(fd, body.c_str(), body.size());
			// 	body_filename = fileName;
			// }

			//print();
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
			body_filename = rhs.getBodyFilename();
			boundary = rhs.getBoundary();
			initRepresentationHeaders();
			initRequestHeaders();
			return *this;
		}

		// parsing

		// int parse_request(std::string raw_req){

		// 	vec req_split_body = split_to_lines(raw_req,"\r\n\r\n");
		// 	if (req_split_body.size() != 2)
		// 		return (1);
		// 	body = req_split_body[1];

		// 	//including first line
		// 	vec head = split_to_lines(req_split_body[0]);

		// 	//parse first line
		// 	std::string first_line = head[0]; // line 1
		// 	int ret = parse_first_line(first_line);
		// 	if (ret) return ret;

		// 	//parse headers
		// 	head.erase(head.begin());
		// 	for (iter it = head.begin(); it != head.end(); ++it)
		// 	{
		// 		vec header = split_to_lines(*it, ": ");
		// 		headers[header[0]] = header[1];
		// 	}
		// 	return (0);
		// }
		
		int		parse_request(std::string buffer)
		{
			// int		i = 0;
			vec		split_ret;
			vec		lines = split_first(buffer, '\n');
			
			// Initializing method, path and query

			int ret = parse_first_line(lines[0]);
			if (ret) return ret;
			size_t i = skip_buff(buffer, 0);
			
			// Initializing Headers

			while (i < buffer.length())
			{
				if (buffer[i] == '\r' && buffer[i + 1] == '\n')
				{
					split_ret = split_first(headers["Content-Type"], ';');
					if (split_ret.size() < 2)
					{
						i = skip_buff(buffer, i);
						break;
					}
					// Trimming the string to compare it to the boundary later
					boundary = trim(split_first(split_ret[1], '=')[1], "-");

					headers["Content-Type"] = split_ret[0]; // To not enter this condition again
					i = skip_buff(buffer, i);
					i = skip_buff(buffer, i);
					continue ;
				}

				lines = split_first(buffer.substr(i, buffer.length()), '\n');
				split_ret = split_first(lines[0], ':');
				headers[split_ret[0]] = split_ret[1];
				i = skip_buff(buffer, i);
			}

			// Initializing Body

			for (; i < buffer.length(); i++)
			{
				if (buffer[i] == '-' && boundary != "" && !boundary.compare(0, boundary.length() - 1, trim(buffer.substr(i, buffer.find("\n", i)), "-\n\r")))
				{
					break;
				}
				body += buffer[i];
			}

			std::cout << "body size = " << body.length() << std::endl;

			return 0;
		}

		size_t	skip_buff(std::string buf, size_t i)
		{
			for (; buf[i] != '\r'; i++)
				;

			return (i += 2);
		}

		size_t	not_from_boundary(std::string str, size_t i)
		{
			for (; i < str.size(); i++)
				if (str[i] != '-' && (str[i] > '9' || str[i] < '0'))
					break;
			return i;
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
		const std::map<std::string, std::string> &getRepresentationHeaders() const {
			return representation_headers;
		}
		const std::map<std::string, std::string> &getRequestHeaders() const {
			return request_headers;
		}
		const std::string getHeader(std::string const &key) const {
			return headers.count(key) ? headers.at(key) : "";
		}
		const std::string &getBody() const {
			return body;
		}
		const std::string &getBodyFilename() const {
			return body_filename;
		}
		const std::string &getBoundary() const {
			return boundary;
		}

		void initRequestHeaders(){
			for (std::map<std::string, std::string>::iterator it = headers.begin(); it!=headers.end(); ++it){
				// if (std::find(it->first.begin(),it->first.end(), "Content-") == it->first.begin()){
				if (std::strstr(it->first.c_str(),"Accept") == it->first.c_str()){
					request_headers.insert(*it);
				}
			}
		}

		void initRepresentationHeaders(){
			for (std::map<std::string, std::string>::iterator it = headers.begin(); it!=headers.end(); ++it){
				// if (std::find(it->first.begin(),it->first.end(), "Content-") == it->first.begin()){
				if (std::strstr(it->first.c_str(),"Content-") == it->first.c_str()){
					representation_headers.insert(*it);
				}
			}
		}
		// debug
		void print() const {
			std::cout 
				<< method << " " 
				<< path << " " 
				<< query << " " 
				<< protocol << "/" 
				<< version <<"  ";
			if (DEBUG)
			{
				std::cout << "\nHeaders:" << std::endl;
				for(std::map<std::string,std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it){
					std::cout << " " << it->first << " " << it->second << std::endl;
				}
				std::cout << "\nBody:\n" <<body<< std::endl;
				//
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
