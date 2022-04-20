#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
 #include <fcntl.h>
extern char **environ;

//convert to string
std::string to_string(int n)
{
    std::ostringstream s;
    s << n;
    return s.str();
}

int main()
{
	int fd = open("test", O_RDWR | O_CREAT, 0777);
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
        dup2(fd, 1);

      		// std::string cgi_location = "/Users/anassif/Desktop/brew/bin/php-cgi";
			// std::string req_file = "test.php";
			std::string cgi_location = "/usr/bin/python";
			std::string req_file = "script.py";
			char *args[3];

			args[0] = (char *)cgi_location.c_str();
			args[1] = (char *)req_file.c_str();
			args[2] = NULL;
			// https://www.cs.ait.ac.th/~on/O/oreilly/perl/perlnut/ch09_04.htm
			std::string method = "GET";
			std::string server_name = "SERVER_NAME";	 // The server's hostname or IP address.
			std::string server_software = "Webserv 1.0"; // The name and version of the server software that is answering the client request.
			// int port = 8080;
			std::string file_path = "sfdsf/sdfds/dsfds.fds";									 // requested file_path
			std::string path_info = "sfdsf/sdfds/dsfds";										 // url until the first "?"
			// std::string query_path = "first_name=hakim&last_name=nassif";//"first_name=yahya&last_name=oukharta";										 // url from the first "?"  to the end
			std::string document_root = "/var/sfdsf/sdfds/home/";								 // The directory from which Web documents are served.
			std::string script_name = "/var/file.php";											 // The path to the executed file
			std::string remote_host = "/var/file.php";											 // The remote hostname of the user making the request, from where the request is made req.get('Host'), example : www.google.com
			std::string remote_address = "875.65.158.33";										 // The remote IP address of the user making the request.
			std::string content_type = "application/x-www-form-urlencoded";												 // The request content type req.get("Content-Type")
			int content_length = 33;															 // The length of the data (in bytes) req.get("Content-Length")
			// std::string accepted_types = "text/html";											 // A list of the MIME types that the client can accept. req.get("Accept")
			std::string user_agent = "Mozilla/5.0 ...";											 // User agent. req.get("User-Agent")
			std::string referer = "http://www.test.com/cgi-bin/test.py?key1=value1&key2=value2"; // The URL of the document that the client points to before accessing the CGI program. req.get("Referer")

			setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
			setenv("SERVER_SOFTWARE", server_software.c_str(), 1);
			setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
			setenv("SERVER_PORT", std::to_string(8000).c_str(), 1);
			setenv("REQUEST_METHOD", "POST", 1);
			setenv("PATH_INFO", "/Users/mac/Desktop/webserv/cgi", 1);
			setenv("PATH_TRANSLATED", file_path.c_str(), 1);
			// setenv("QUERY_STRING", query_path.c_str(), 1);
			setenv("DOCUMENT_ROOT", document_root.c_str(), 1);
			setenv("SCRIPT_NAME", script_name.c_str(), 1);
			setenv("REMOTE_HOST", remote_host.c_str(), 1);
			setenv("REMOTE_ADDR", remote_address.c_str(), 1);
			setenv("CONTENT_TYPE", content_type.c_str(), 1);
			setenv("CONTENT_LENGTH", std::to_string(content_length).c_str(), 1);
			// setenv("HTTP_ACCEPT", accepted_types.c_str(), 1);
			setenv("HTTP_USER_AGENT", user_agent.c_str(), 1);
			setenv("HTTP_REFERER", referer.c_str(), 1);

			if (execve(args[0], args, environ) == -1)
				perror("Could not execve fff");
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

			lseek(fd, 0, SEEK_SET);
			while ((nbytes = read(fd, cgi_buff, 1024)) > 0)
			{
				std::cout << "Got some data from pipe : " << cgi_buff << std::endl;
			}
			close(fd);
		}
		return 0;
	

}