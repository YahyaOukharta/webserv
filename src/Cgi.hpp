#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include "Request.hpp"
#include "Server.hpp"
#include <sys/wait.h>
extern char**	 environ;

class Cgi
{
private:
	Request const &req;
	Server const *server;
	Location const *location;
	std::string const resPath;
	
public:
	Cgi(Request const &_req,Server const* _srv,Location const* _loc, std::string const &_resPath) : req(_req), server(_srv), location(_loc), resPath(_resPath)
	{

	}
	Cgi(Cgi const &src);
	~Cgi() {}

	Cgi &operator=(Cgi const &rhs);

	std::string compile()
	{
		int t1 = time(NULL);
		std::string fileName = "/tmp/compiled_" + std::to_string(t1);
		int fd = open(fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
		std::cout << fileName << std::endl;

		int fork_id = fork();
		if(fork_id == -1)
		{
			//throw "500";
			std::cout << "error on fork forking" << std::endl; 
			throw "500 Internal Server Error";
		}
		else if(fork_id == 0) // child process
		{
			// if R is GET
			// if R is POST dup file disc of body to input
			//pipe is limited because it can hang, and you dont need it since at the end you will put the output in a file.
			dup2(fd, 1);//if this fail it an internal error with an appropriate err number

			std::string cgi_location = location->getCgiPath();
			std::string req_file = resPath;
			char *args[4];
			args[0] = (char *)cgi_location.c_str();
			args[1] = (char *)req_file.c_str();
			args[2] = NULL;

			
			setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
			setenv("SERVER_SOFTWARE", "Webserv 1.0", 1);
			setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
			setenv("SERVER_PORT", std::to_string(server->getConfig().getPort()).c_str(), 1);
			setenv("REQUEST_METHOD", "POST", 1);
			setenv("SCRIPT_FILENAME", req_file.c_str(), 1);
			setenv("PATH_INFO", "/Users/anassif/Desktop/cgi-branch/cgi", 1); //need path info from request
			// setenv("PATH_TRANSLATED", file_path.c_str(), 1);
			setenv("QUERY_STRING", req.getQuery().c_str(), 1);
			// setenv("DOCUMENT_ROOT", document_root.c_str(), 1);
			// setenv("SCRIPT_NAME", req_file.c_str(), 1); //need script name from request
			// setenv("REMOTE_HOST", remote_host.c_str(), 1);
			setenv("REMOTE_ADDR", server->getConfig().getHost().c_str(), 1);
			setenv("CONTENT_TYPE", req.getHeader("Content-Type").c_str(), 1); //need content typr from request
			setenv("CONTENT_LENGTH", req.getHeader("Content-Length").c_str(), 1);  //need content lengh from request
			// setenv("REDIRECT_STATUS", "200", 1);
			setenv("HTTP_ACCEPT", req.getHeader("Accept").c_str(), 1);
			setenv("HTTP_USER_AGENT", req.getHeader("User-Agent").c_str(), 1);
			setenv("HTTP_REFERER", req.getHeader("Referer").c_str(), 1);

			if (execve(args[0], args, environ) == -1)
				throw "500 Internal Server Error";
			}
			else // parent process
			{
				int wstatus;
				// int t1 = time();
				waitpid(fork_id, &wstatus, 0);
				if (WIFEXITED(wstatus))
				{
					int status_code = WEXITSTATUS(wstatus);
					if (status_code != 0)
					{
						std::cout << "Failure with status code : " << status_code << std::endl;
						throw "500 Internal Server Error";
					}
				}
				// int status;
				// int ret = 0;
				// time_t t = time(NULL);
				// while ((time(NULL) - t) < 5) {
				// 	if (waitpid(fork_id, &status, WNOHANG) > 0) {
				// 		if (WIFEXITED(status)) {
				// 			ret = WEXITSTATUS(status);
				// 			break ;
				// 		}
				// 	}
				// }
				// if (kill(fork_id, SIGKILL) == 0) {
				// 	remove(fileName.c_str());
				// 	throw "500 Internal Server Error";
				// }

				// int nbytes;
				// char cgi_buff[1024] = {0};
				// // //to reset cursor to first byte of the file
				// lseek(fd, 0, SEEK_SET);
				// nbytes = read(fd, cgi_buff, 1024);
				// std::string line(cgi_buff);
				// if (line.find("Status: ")!= std::string::npos)
				
				
			}
			close(fd);
			return fileName;
	}
};

std::ostream &operator<<(std::ostream &o, Cgi const &i);

#endif /* ************************************************************* CGI_H */
