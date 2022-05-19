#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include <cstring>
#include "Utils.hpp"
# include <fcntl.h>
# define DEBUG 0
# include <sys/time.h>
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

		std::string 						body;
		std::string 						body_filename;

		size_t								req_time;
		std::string							boundary;


		int state; // 0 for new, 1 for first line done, 2 for headers done, 3 for body done
		std::string pending_buf;
		size_t chunk_size;
		size_t chunk_size_update;

		std::vector<size_t> boundary_idx;


	public:

		Request(){
			state = 0;
			chunk_size = -1;
			chunk_size_update = -1;
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
				if (error == 4)
					throw webserv_exception("Not done yet");
			}
			initRepresentationHeaders();
			initRequestHeaders();
			if (method=="POST")
			{
				int t1 = time(NULL);
				std::string fileName = "/tmp/body_" + ft::itoa(t1);
				int fd = open(fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
				write(fd, body.c_str(), body.size());
				body_filename = fileName;
				close(fd);
			}
			req_time = time(NULL);
			// std::cout << req_time << std::endl;
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
			req_time = rhs.getTime();
			boundary = rhs.getBoundary();
			boundary_idx = rhs.boundary_idx;
			initRepresentationHeaders();
			initRequestHeaders();
			return *this;
		}

		// parsing

		void setReqTimeAndBodyToFile(){
			req_time = time(NULL);
			if (method=="POST")
			{
				std::string fileName = "/tmp/body_" + ft::itoa(req_time);
				int fd = open(fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
				write(fd, body.c_str(), body.size());
				body_filename = fileName;
				close(fd);
			}
		}

		int handle_request_update(char *buf, size_t s) {

			//std::cout << std::string(buf,s) << ">> size "<< s << std::endl;
			pending_buf.append(buf,s);

			size_t n;
			if (state == 0){
				// find end of first line
	
				if ((n = pending_buf.find("\r\n")) != std::string::npos){
					int ret = parse_first_line(pending_buf.substr(0,n));
					if (ret)
					{
						//std::cout << "something wrong with first line" << std::endl;
					}
					else
					{
						pending_buf.erase(pending_buf.begin(),pending_buf.begin()+ n + 2);
						state++;
						parseUrl();
						parseQuery();
						// std::cout << "done parsing first line" << std::endl;
						//return (0);
					}
				}
			}
			if (state == 1){
				// find end of first line
	
				if ((n = pending_buf.find("\r\n\r\n")) != std::string::npos){
					std::string headers_buf = pending_buf.substr(0,n);
					vec head = split_to_lines(headers_buf); 
					for (iter it = head.begin(); it != head.end(); ++it)
					{
						vec header = split_to_lines(*it, ": ");
						headers[header[0]] = header[1];
					}
					pending_buf.erase(pending_buf.begin(),pending_buf.begin()+ n + 4);
					state++;
					if (!headers["Content-Type"].compare(0, 19, "multipart/form-data"))
					{
						vec	split_ret = split_first(headers["Content-Type"], ';');
						boundary = trim(split_first(split_ret[1], '=')[1], "-");
					}
					// std::cout << "done parsing headers" << std::endl;
					//return (0);
				}
			}
			if (state == 2){
				if (body.capacity() < (u_int)ft::atoi(headers["Content-Length"].c_str()))
					body.reserve((u_int)ft::atoi(headers["Content-Length"].c_str()));
				if (headers["Transfer-Encoding"] != "chunked")
				{
					body.append(pending_buf);
					pending_buf.clear();
					if(body.size() >= (u_int)ft::atoi(headers["Content-Length"].c_str()))
					{
						state++;
						setReqTimeAndBodyToFile();
					}
				}
				while(pending_buf.size())
				{	
					if(chunk_size_update != (size_t)-1)
					{
						if(chunk_size_update == 0 && pending_buf.size() >= 2)
						{
							chunk_size_update = -1;
							chunk_size = -1;
							pending_buf.erase(pending_buf.begin(), pending_buf.begin() + 2);
						}
						else
						{
							std::string sub = pending_buf.substr(0, MIN(chunk_size_update, pending_buf.size()));
							body.append(sub);
							pending_buf.erase(pending_buf.begin(), pending_buf.begin()+ sub.size());
							chunk_size_update -= sub.size();
						}
					}
					if (chunk_size == (size_t)-1 && (n = pending_buf.find("\r\n")) != std::string::npos){
						chunk_size = std::stol(pending_buf.substr(0,n),0,16);
						chunk_size_update = chunk_size;
						pending_buf.erase(pending_buf.begin(),pending_buf.begin() + n + 2);
						if(chunk_size_update == 0)
						{
							state++;
							setReqTimeAndBodyToFile();
						}
					}
				}
			}
			
			if(state == 3){
				if(boundary != "")
				{
					size_t bound;
					while ((bound = body.find(boundary, (boundary_idx.size() ? boundary_idx[boundary_idx.size() - 1] + 1 : 0 ))) != std::string::npos){
						boundary_idx.push_back(bound);
					}
				}
				// for(std::vector<size_t>::iterator it = boundary_idx.begin(); it!= boundary_idx.end(); ++it){
				// 	std::cout << "->" << *it << std::endl;
				// }
				//print();
			}
			//print();
			// std::cout << "body size = " << ((float)body.size() / (float)ft::atoi(headers["Content-Length"].c_str()))*100 <<"%"<< std::endl;
			return (0);
		}

		int parse_request(std::string raw_req){

			vec req_split_body = split_to_lines(raw_req,"\r\n\r\n");

			if (req_split_body.size() < 2)
				return (1);
			body = raw_req.substr(req_split_body[0].size()+4);

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
				vec header = split_to_lines(*it, ": ");
				headers[header[0]] = header[1];
			}
			// std::cout << "BODY = \n" << body << std::endl;
			// std::cout <<"not done yet "<< body.size() << " " << (u_int)ft::atoi(headers["Content-Length"].c_str()) << std::endl;

			if(headers["Content-Length"] != "")
			{
				if (body.size() < (u_int)ft::atoi(headers["Content-Length"].c_str()))
				{
					// std::cout <<"not done yet "<< body.size() << " " << (u_int)ft::atoi(headers["Content-Length"].c_str()) << std::endl;
					return (4);
				}
				if (!headers["Content-Type"].compare(0, 19, "multipart/form-data"))
				{
					vec	split_ret = split_first(headers["Content-Type"], ';');
					boundary = trim(split_first(split_ret[1], '=')[1], "-");
				}
			}
			parseUrl();
			parseQuery();
			std::cout << "request "<< path <<" done" << std::endl;
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
		
		void parseUrl(){
			size_t n = 0;
			std::string digits = "0123456789ABCDEF";
			while((n = path.find('%', n )) != std::string::npos)
			{
				if(n + 2 < path.size()
			 	&& digits.find(path[n+1]) != std::string::npos
			 	&& digits.find(path[n+2]) != std::string::npos)
				{
					std::string sp = path.substr(n+1,2);
					unsigned int x;   
					std::stringstream ss;
					ss << std::hex << sp;
					ss >> x;
					path.replace(n, 3,std::string(1,(char)x));
				}
				else n++;
			}
		}
		void parseQuery(){
			size_t n = 0;
			std::string digits = "0123456789ABCDEF";
			while((n = query.find('%', n )) != std::string::npos)
			{
				if(n + 2 < path.size()
			 	&& digits.find(path[n+1]) != std::string::npos
			 	&& digits.find(path[n+2]) != std::string::npos)
				{
					std::string sp = query.substr(n+1,2);
					unsigned int x;   
					std::stringstream ss;
					ss << std::hex << sp;
					ss >> x;
					query.replace(n, 3,std::string(1,(char)x));
				}
				else n++;
			}
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
		void addSlashToPath(){
			path+='/';
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
		size_t getTime() const {
			return req_time;
		}
		const std::string &getBoundary() const {
			return boundary;
		}
		int	getState() const {
			return state;
		}
		const std::vector<size_t>	&getIndexes() const {	return boundary_idx;	}
		void initRequestHeaders(){
			for (std::map<std::string, std::string>::iterator it = headers.begin(); it!=headers.end(); ++it){
				// if (std::find(it->first.begin(),it->first.end(), "Content-") == it->first.begin()){
				if (std::strstr(it->first.c_str(),"Accept") == it->first.c_str()) {
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
		void print(int no_endl = 0) const {
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
				std::cout << "\nBody size:\n" <<body<< std::endl;
				//
				return;
			}
			std::cout << headers.size() << " headers   ";
			if (body.size())
				std::cout << "Body size : " << body.size() << " bytes";
			else
				std::cout << "Empty body";

			struct timeval time_now;
			gettimeofday(&time_now, nullptr);
			time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
			std::cout <<" "<< msecs_time - req_time*1000<<"ms";
			if (!no_endl)
			std::cout << std::endl;
		}

	private:

};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */
