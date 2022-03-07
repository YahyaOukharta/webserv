#include "config-file.hpp"
#include "string-utils.hpp"

// int config::check_if_random(const std::string &s)
// {
     
// }
void config::parse_buffer(const std::string &s)
{
    // std::cout << s << std::endl;
    server serv;
    std::string str;
    if(s[0] == '[')
    {
        str = s.substr(s.find("[") + 1, s.find("]") - 1);
        if (str != "end")
        {
            
            // std::cout << str << std::endl; // got servernames
            serv.name = str;
            index_loc = 0;
            _servers.push_back(serv);
            std::cout << _servers[0].name << std::endl;
        }
        else if(str == "end"){
            // std::cout << "dwadawdawdawd\n";
            index++;
        }
    }
    else{
        // if(check_if_random())
            // std::cout << "wtf you typing\n";return ;
        switch(s[0]){
            case 'h':{
                if(s.find("host = ") != std::string::npos)
                {   str = s.substr(s.find("host = ") + 7, s.length() - 7);
                    str = str.substr(0, str.length() - 1);
                    _servers[index].host = str;
                    // std::cout << _servers[index].host << std::endl;
                    }
                //   else
                //     throw std::invalid_argument( "bad argumet \n" );
                break;
            }
            case 'p' :{
                if(s.find("port = ") != std::string::npos){
                str = s.substr(s.find("port = ") + 7, s.length() - 7);
                str = str.substr(0, str.length() - 1);
                _servers[index].port = ft::atoi(str.c_str());
                // std::cout << _servers[index].port << std::endl;
                }
              
                break;
            }
            case 'd': {
                if(s.find("default_error_pages = ") != std::string::npos){
                str = s.substr(s.find("default_error_pages = ") + 22, s.length() - 22);
                str = str.substr(0, str.length() - 1);
                _servers[index].root = str;
                // std::cout << _servers[index].root << std::endl;
                }
                break;
            }
            case 'b': {
                if(s.find("bodysize_limit = ") != std::string::npos){
                str = s.substr(s.find("bodysize_limit = ") + 17, s.length() - 17);
                str = str.substr(0, str.length() - 1);
                _servers[index].bodysize_limit = ft::atoi(str.c_str());
                // std::cout << _servers[index].bodysize_limit << std::endl;
                }
                break;
            }
            case 'l': {
                if(s.find("location = ") != std::string::npos){
                str = s.substr(s.find("location = ") + 11, s.length() - 11);
                str = str.substr(0, str.length() - 1);
                // _servers[index].location = str;
                //get infos from location line
                std::cout << str << std::endl;}
                break;
            }
            
        }
    }
}
config::config(const std::string s)
{
    //thisi s for allocation of servers later
    count_servers(s);
    index = 0;
    // std::cout << server_count << std::endl;
    //
    std::ifstream       file(s);
    if (file)
    {

        std::stringstream os;
        std::string buffer;

        os << file.rdbuf();
        buffer = os.str();

        std::stringstream a(buffer);

        while(std::getline(a, buffer))
        {
            // std::cout << buffer << std::endl;
            ft::ltrim(buffer);
            ft::rtrim(buffer);
            // std::cout << buffer << std::endl;
            if(ft::isSkippable(buffer))
                continue ;
            size_t pos = buffer.find_first_of('#');
            if(pos != std::string::npos)
                buffer = buffer.substr(0, pos);
            ft::ltrim(buffer);ft::rtrim(buffer);
            parse_buffer(buffer);
            
        }
    }
}
void config::count_servers(const std::string &s)
 {
    std::ifstream       file(s);
    std::string str;
    server_count = 0;
    if (file)
    {

        std::stringstream os;
        std::string buffer;

        os << file.rdbuf();
        buffer = os.str();

        std::stringstream a(buffer);

        while(std::getline(a, buffer))
        {
            ft::ltrim(buffer);
            ft::rtrim(buffer);
            if(ft::isSkippable(buffer))
                continue ;
            if(buffer[0] == '[')
            {
                str = buffer.substr(buffer.find("[") + 1, buffer.find("]") - 1);
                if(str == "end"){
                    server_count++;
                }
            }
        }
    }
 }

config::~config(){};