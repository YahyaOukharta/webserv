#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include "Request.hpp"
#include "Server.hpp"

extern char**	 environ;

class Cgi
{
private:
	Request const &req;
	Server const *server;
	std::string const &resPath;
public:
	Cgi(Request const &_req,Server const* _srv, std::string const &_resPath) : req(_req), server(_srv), resPath(_resPath)
	{

	}
	Cgi(Cgi const &src);
	~Cgi() {}

	Cgi &operator=(Cgi const &rhs);

	int compile(std::string fileName)
	{
		int fd = open(fileName, O_RDWR | O_CREAT, 0777);
		int fork_id = fork();
		if(fork_id == -1)
		{
			std::cout << "error on fork forking" << std::endl; 
			return (1);
		}
		else if(fork_id == 0) // child process
		{
			// if R is GET
			// if R is POST dup file disc of body to input
			//pipe is limited because it can hang, and you dont need it since at the end you will put the output in a file.
			dup2(fd, 1);//if this fail it an internal error with an appropriate err number

				//if else here to determine which interpreter we will be using after getting extension from request
				// std::string cgi_location = "/Users/anassif/Desktop/brew/bin/php-cgi";
				// std::string req_file = "test.php";

				std::string cgi_location = "/Users/anassif/Desktop/brew/bin/php-cgi";
				std::string req_file = "test.php";
				char *args[3];

				args[0] = (char *)cgi_location.c_str();
				args[1] = (char *)req_file.c_str();
				args[2] = NULL;
			
				setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
				setenv("SERVER_SOFTWARE", "Webserv 1.0", 1);
				setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
				setenv("SERVER_PORT", std::to_string(_srv.get_server_port()).c_str(), 1);
				setenv("REQUEST_METHOD", req.getMethod(), 1);
				setenv("PATH_INFO", "/Users/mac/Desktop/webserv/cgi", 1); //need path info from request
				// setenv("PATH_TRANSLATED", file_path.c_str(), 1);
				setenv("QUERY_STRING", req.getQuery(), 1);
				// setenv("DOCUMENT_ROOT", document_root.c_str(), 1);
				setenv("SCRIPT_NAME", script_name.c_str(), 1); //need script name from request
				// setenv("REMOTE_HOST", remote_host.c_str(), 1);
				setenv("REMOTE_ADDR", _srv.get_server_host(), 1);
				setenv("CONTENT_TYPE", content_type.c_str(), 1); //need content typr from request
				setenv("CONTENT_LENGTH", std::to_string(content_length).c_str(), 1);  //need content lengh from request
				// setenv("HTTP_ACCEPT", accepted_types.c_str(), 1);
				// setenv("HTTP_USER_AGENT", user_agent.c_str(), 1);
				setenv("HTTP_REFERER", referer.c_str(), 1);

				if (execve(args[0], args, environ) == -1)
					perror("Could not execve fff");//this needs to be changed for and exit with an error number i guess
			}
			else // parent process
			{
				int wstatus;
				waitpid(fork_id, &wstatus, 0);
				if (WIFEXITED(wstatus))
				{
					int status_code = WEXITSTATUS(wstatus);
					if (status_code != 0)
					{
						std::cout << "Failure with status code : " << status_code << std::endl;
						return (1);
					}
				}

				int nbytes;
				char cgi_buff[1024] = {0};
				//to reset cursor to first byte of the file
				lseek(fd, 0, SEEK_SET);
				while ((nbytes = read(fd, cgi_buff, 1024)) > 0)
				{
					std::cout << "Got some data from file : " << cgi_buff << std::endl;
				}
			}
			close(fd);
			return 0;
	}
};

std::ostream &operator<<(std::ostream &o, Cgi const &i);

#endif /* ************************************************************* CGI_H */
