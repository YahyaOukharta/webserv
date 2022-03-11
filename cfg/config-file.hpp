#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>

class config
{
    public:
		config();
		config(std::string file);
		config &operator=(const config &obj);
		~config();
        
        private:
        struct location //this going to be a class with there geters
        {
            std::string name;
            std::string path;
            std::string root;
            std::string index;
            std::vector<std::string> methods;  // allowed methodes
            std::string autoindex;
            
            std::string cgi_path;
            
            std::string upload_path;
            size_t client_max_body_size;
        };
        struct server
        {
            std::string name; // containes name of server
            std::string host;
            size_t port;
            size_t bodysize_limit;
            std::string root;
            std::map<int, std::string> error_pages; //int for error number string for path
            std::vector<location> locations; // containes all locations of a server
            
        };
        public:
        size_t server_count;
        size_t index;
        size_t index_loc;
        std::vector<server> _servers; // containes all the servers

        void parse_buffer(const std::string &s);
        void count_servers(const std::string &s);
        int check_if_random(const std::string &s);
        void multi_spliter(std::string &s, const std::string &);
        std::string normal_split(const std::string &line, const std::string &spliter);
        //getters and setters
        

    //need methodes to facilitate the parsing cant think of any or how now
};