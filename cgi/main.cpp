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

int main(int argc, char **argvg)
{
    int pipe_fd[2];
    // pipe_fd[0] - read
    // pipe_fd[1] - wrtie
    // file descriptor 0 : stdin
    // file descriptor 1 : stdout
    // file descriptor 2 : stderr

    if(pipe(pipe_fd) == -1)
    {
        std::cout << "error on pipe pipe" << std::endl;
        return (1);
    }

    int fork_id = fork();
    if(fork_id == -1) // child process
    {
        std::cout << "error on fork forking" << std::endl;
        return (1);
    }
    else if(fork_id == 0) // child process
    {
        close(pipe_fd[0]); // close the read end
        dup2(pipe_fd[1], 1);

        std::string cgi_location = "/Users/anassif/Desktop/brew/bin/php-cgi";
        std::string req_file = "test.php";
        char *args[3];
       
        args[0] = (char *)cgi_location.c_str();
        args[1] = (char *)req_file.c_str();
        args[2] = NULL;
       
        std::string method = "GET";
        std::string server_name = "SERVER_NAME"; // The server's hostname or IP address.
        std::string server_software = "Webserv 1.0"; // The name and version of the server software that is answering the client request.
        int port = 8080;
        std::string file_path = "sfdsf/sdfds/dsfds.fds"; // requested file_path
        std::string path_info = "sfdsf/sdfds/dsfds"; // url until the first "?" 
        std::string query_path = "?sfdsf/sdfds/dsfds"; // url from the first "?"  to the end
        std::string document_root = "/var/sfdsf/sdfds/home/"; // The directory from which Web documents are served.
        std::string script_name = "/var/file.php"; // The path to the executed file
        std::string remote_host = "/var/file.php"; // The remote hostname of the user making the request, from where the request is made req.get('Host'), example : www.google.com
        std::string remote_address = "875.65.158.33"; // The remote IP address of the user making the request.
        std::string content_type = "text/html"; // The request content type req.get("Content-Type")
        int content_length = 520; // The length of the data (in bytes) req.get("Content-Length")
        std::string accepted_types = "text/html"; // A list of the MIME types that the client can accept. req.get("Accept")
        std::string user_agent = "Mozilla/5.0 ..."; // User agent. req.get("User-Agent")
        std::string referer = " https://www.cplusplus.com/reference/vector/vector/?kw=vector...."; // The URL of the document that the client points to before accessing the CGI program. req.get("Referer")
        
     
        setenv("GATEWAY_INTERFACE", "=CGI/1.1", 1);
        setenv("SERVER_SOFTWARE", server_software.c_str(), 1);
        setenv("SERVER_PROTOCOL","=HTTP/1.1", 1 );
        setenv("SERVER_PORT", to_string(8000).c_str(), 1);
        setenv("REQUEST_METHOD", "GET", 1);
        setenv("PATH_INFO", "/Users/mac/Desktop/webserv/cgi", 1);
        setenv("PATH_TRANSLATED", file_path.c_str(), 1);
        setenv("QUERY_STRING", query_path.c_str(), 1);
        setenv("DOCUMENT_ROOT", document_root.c_str(), 1);
        setenv("SCRIPT_NAME", script_name.c_str(), 1);
        setenv("REMOTE_HOST", remote_host.c_str(), 1);
        setenv("REMOTE_ADDR", remote_address.c_str(), 1);
        setenv("CONTENT_TYPE", content_type.c_str(), 1);
        setenv("CONTENT_LENGTH", to_string(content_length).c_str(), 1);
        setenv("HTTP_ACCEPT", accepted_types.c_str(), 1);
        setenv("HTTP_USER_AGENT", user_agent.c_str(), 1);
        setenv("HTTP_REFERER", referer.c_str(), 1);

        std::cout << args[0] << "---" << args <<  std::endl;
        if (execve(args[0], args, NULL) == -1)
            perror("Could not execve fff");

        close(pipe_fd[1]); // close the write end after finishing writing
    }
    else // parent process
    {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus))
        {
            int status_code = WEXITSTATUS(wstatus);
            if(status_code != 0)
            {
                std::cout << "Failure with status code : " << status_code << std::endl;
                return (1);
            }
        }


        close(pipe_fd[1]); // close the write end because we don't need it
        int nbytes;
        char cgi_buff[1024] = {0};

        // Read the data from pipe_fd[0], and search for EOF or content_length
        while ((nbytes = read(pipe_fd[0], cgi_buff, 1024)) > 0) { 
                std::cout << "Got some data from pipe : " << cgi_buff << std::endl;
        }
        close(pipe_fd[0]); // close the read end after finishing reading
    }
    //need to change all this to just output on a file or a string.

    return (0);
}