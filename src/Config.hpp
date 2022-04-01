#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"

class Config
{
    public:
		Config();
		Config(std::string file);
		Config &operator=(const Config &obj);
		~Config();
        
        struct location //this going to be a class with there geters
        {
            
            std::string path;
            std::string root;
            std::string index;
            std::string method;
            // std::vector<std::string> methods;  // allowed methodes maybe for later
            std::string autoindex;
            std::string cgi_path;
            std::string extension;
            std::string upload_path;
            int bodysize_limit;
        };
        struct serveur
        {
            std::string name; // containes name of server
            std::string host;
            int port;
            int bodysize_limit;
            std::string root;
            std::string default_error_pages;
            // std::map<int, std::string> error_pages; //int for error number string for path for later
            std::vector<location> locations; // containes all locations of a server
            
        };
        public:
        size_t server_count;
        size_t index;
        size_t checker;
        std::vector<serveur> _servers; // containes all the servers

        void parse_buffer(const std::string &s);
        void count_servers(const std::string &s);
        int check_if_random(const std::string &s);
        void parse_location(std::string &s, const std::string &);
        std::string normal_split(const std::string &line, const std::string &spliter);
        void set_defaults(size_t);
        void set_defaults_2(location *l);
        int check_args();
        //getters and setters

        //this will get you all data of all locations of a server
        friend std::vector<Config::location> get_all_locations(const Config::serveur &s)
        {
            return (s.locations);
        }
        friend std::string get_server_name(const Config::serveur &s)
        {
            return s.name;
        }
        friend std::string get_server_host(const Config::serveur &s)
        {
            return s.host;
        }
        friend int get_server_port(const Config::serveur &s)
        {
            return s.port;
        }
        //you can add more if needed

    //need methodes to facilitate the parsing cant think of any or how now
    
};

#endif

