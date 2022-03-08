#include "config-file.hpp"
#include "string-utils.hpp"

void config::multi_spliter(std::string &line)
 {
    ft::ltrim(line);
    ft::rtrim(line);
    std::vector<std::string> s;
    location l;
    std::size_t prev = 0, pos;
    std::string str;
    while ((pos = line.find_first_of(",", prev)) != std::string::npos)
    {
        if (pos > prev)
        {    
            str = line.substr(prev, pos-prev);
            ft::ltrim(str);
            s.push_back(str);
        }
        prev = pos + 1;
    }
    if (prev < line.length())
    {   
        str = line.substr(prev, std::string::npos);
        ft::ltrim(str);
        s.push_back(str);
    }
    pos = -1;
    while(++pos < s.size())
    {
        // std::cout <<   s[pos][1] << std::endl;
        switch(s[pos][0]){
           
            case 'p':{
                if(s[pos].find("path") != std::string::npos)
                {   str = s[pos].substr(s[pos].find("path") + 7, s[pos].length());
                    str = str.substr(0, str.length());
                    // _servers[index].host = str;
                    l.path = str;
                    // std::cout << str << std::endl;
                    // std::cout << _servers[index].host << std::endl;
                    }
                //   else
                //     throw std::invalid_argument( "bad argumet \n" );
                break;
            }
            case 'm' :{
                if(s[pos].find("method") != std::string::npos){
                str = s[pos].substr(s[pos].find("method") + 9, s[pos].length());
                str = str.substr(0, str.length());
                l.methods.push_back(str);
                // std::cout << str << std::endl;
                // _servers[index].port = ft::atoi(str.c_str());
                // std::cout << _servers[index].port << std::endl;
                }
              
                break;
            }
            case 'r' :{
                if(s[pos].find("root") != std::string::npos){
                str = s[pos].substr(s[pos].find("root") + 7, s[pos].length());
                str = str.substr(0, str.length());
                l.root = str;
                // std::cout << str << std::endl;
                // _servers[index].port = ft::atoi(str.c_str());
                // std::cout << _servers[index].port << std::endl;
                }
              
                break;
            }
            case 'a' :{
                if(s[pos].find("autoindex") != std::string::npos){
                str = s[pos].substr(s[pos].find("autoindex") + 12, s[pos].length());
                str = str.substr(0, str.length());
                l.autoindex = str;
                // std::cout << str << std::endl;
                // _servers[index].port = ft::atoi(str.c_str());
                // std::cout << _servers[index].port << std::endl;
                }
              
                break;
            }
            
        }
        
    }
    _servers[index].locations.push_back(l);
 }
 
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
<<<<<<< HEAD
            std::cout << _servers[index].name << "<================="<< std::endl;
=======
            //std::cout << _servers[0].name << std::endl;
>>>>>>> 615e4f4d9effebc48f8a322678fab38dae60730a
        }
        else{
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
<<<<<<< HEAD
                str = s.substr(s.find("location = ") + 11, s.length() - 11);
                str = str.substr(0, str.length() - 1);
                multi_spliter(str);
                // _servers[index].location = str;
                //get infos from location line
                // std::cout << str << std::endl;}
=======
                    str = s.substr(s.find("location = ") + 11, s.length() - 11);
                    str = str.substr(0, str.length() - 1);
                    // _servers[index].location = str;
                    //get infos from location line
                    //std::cout << str << std::endl;
                }
>>>>>>> 615e4f4d9effebc48f8a322678fab38dae60730a
                break;
            }
            
        }
    }
}
}
config::config(const std::string s)
{
    //thisi s for allocation of servers later
    // count_servers(s);
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
    size_t i = -1;
    while(++i < _servers[0].locations.size())
    {
        std::cout << _servers[0].locations[i].path + "  " + _servers[0].locations[i].root + "  " + _servers[0].locations[i].autoindex;
        size_t j = -1;
        while(++j < _servers[0].locations[i].methods.size())
        {
            std::cout << "  " + _servers[0].locations[i].methods[j] << " ";
        }
        std::cout << std::endl;
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