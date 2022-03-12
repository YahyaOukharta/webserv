#include "config-file.hpp"
#include "string-utils.hpp"
std::string config::normal_split(const std::string &line, const std::string &spliter)
{
    std::string s  = line;
    ft::ltrim(s);
    ft::rtrim(s);
    s = s.substr(0 , s.find_first_of(spliter) - 1);
    return s;
}
void config::multi_spliter(std::string &line, const std::string &spliter)
 {
    ft::ltrim(line);
    ft::rtrim(line);
    std::vector<std::string> s;
    location l;
    std::size_t prev = 0, pos;
    std::string str;

    while ((pos = line.find_first_of(spliter, prev)) != std::string::npos)
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
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("path = ") != std::string::npos && test == "path")
                {
                    str = s[pos].substr(s[pos].find("path = ") + 7, s[pos].length());
                    str = str.substr(0, str.length());
                    l.path = str;
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'm' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("method = ") != std::string::npos && test == "method")
                {
                    
                    {str = s[pos].substr(s[pos].find("method = ") + 9, s[pos].length());
                    str = str.substr(0, str.length());
                    l.methods.push_back(str);}
                    
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'r' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("root = ") != std::string::npos && test == "root")
                {
                    {str = s[pos].substr(s[pos].find("root = ") + 7, s[pos].length());
                    str = str.substr(0, str.length());
                    l.root = str;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'a' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("autoindex = ") != std::string::npos && test == "autoindex")
                {
                    {str = s[pos].substr(s[pos].find("autoindex = ") + 12, s[pos].length());
                    str = str.substr(0, str.length());
                    l.autoindex = str;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            default:
                throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");
                break;
        }
        
    }
    _servers[index].locations.push_back(l);
 }
 
void config::parse_buffer(const std::string &s)
{
    // std::cout << s << std::endl;
    server serv;
    std::string str;
    
    
    if(s.find(";") != std::string::npos){
    std::string splited = s.substr(s.find_first_of(";") + 1, s.length() - 1);
    ft::ltrim(splited); ft::rtrim(splited);
    if(!ft::isSkippable(splited))
        {throw std::invalid_argument( "bad argumet :" + s + "\n");}}
    if(s[0] == '[')
    {
        if(s[s.length() - 1] != ']')
        {throw std::invalid_argument( "bad argumet :" + s + "\n");}
        str = s.substr(s.find("[") + 1, s.find("]") - 1);
        if (str != "end")
        {
            serv.name = str;
            // index_loc = 0;
            checker = 1;
            _servers.push_back(serv);
            
            std::cout << "==================>" + _servers[index].name << "<================="<< std::endl;
        }
        else{
            index++;
            checker = 0;
        }
    }
    else{
        str = s;
        ft::rtrim_space(str);
        if(str[str.length() - 1] != ';')
        {throw std::invalid_argument( "bad argumet :" + str + "\n");}
        switch(s[0]){
            case 'h':{
                std::string test = normal_split(s, "=");
                if(s.find("host = ") != std::string::npos && test == "host")
                {
                    {str = s.substr(s.find("host = ") + 7, s.length() - 7);
                    str = str.substr(0, str.length() - 1);
                    _servers[index].host = str;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'p' :{
                std::string test = normal_split(s, "=");
                if(s.find("port = ") != std::string::npos && test == "port")
                {
                    
                    {str = s.substr(s.find("port = ") + 7, s.length() - 7);
                    str = str.substr(0, str.length() - 1);
                    _servers[index].port = ft::atoi(str.c_str());
                    }
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'd': {
                std::string test = normal_split(s, "=");
                if(s.find("default_error_pages = ") != std::string::npos && test == "default_error_pages")
                {
                    
                    {str = s.substr(s.find("default_error_pages = ") + 22, s.length() - 22);
                    str = str.substr(0, str.length() - 1);
                    _servers[index].root = str;}
                // std::cout << _servers[index].root << std::endl;
                }
                 else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'b': {
                std::string test = normal_split(s, "=");
                if(s.find("bodysize_limit = ") != std::string::npos && test == "bodysize_limit")
                {
                   
                    {str = s.substr(s.find("bodysize_limit = ") + 17, s.length() - 17);
                    str = str.substr(0, str.length() - 1);
                    _servers[index].bodysize_limit = ft::atoi(str.c_str());}
                // std::cout << _servers[index].bodysize_limit << std::endl;
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'l': {
                std::string test = normal_split(s, "=");
                if(s.find("location = ") != std::string::npos && test == "location")
                {
                    // if(normal_split(s, "=") == "location")
                    {str = s.substr(s.find("location = ") + 11, s.length() - 11);
                    str = str.substr(0, str.length() - 1);
                    multi_spliter(str, ",");}
                // _servers[index].location = str;
                //get infos from location line
                // std::cout << str << std::endl;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
        }
        default:
            throw std::invalid_argument( "bad argumet :" + s + "\n");
            break;
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
    if(checker != 0)
    {
        throw std::invalid_argument( "config file syntax is not proper\n");
    }
    size_t i = -1;
    // std::cout << _servers[1].locations.size() << std::endl;
    while(++i < _servers[1].locations.size())
    {
        std::cout << _servers[1].locations[i].path + "  " + _servers[1].locations[i].root + "  " + _servers[1].locations[i].autoindex;
        size_t j = -1;
        while(++j < _servers[1].locations[i].methods.size())
        {
            std::cout << "  " + _servers[1].locations[i].methods[j] << " ";
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