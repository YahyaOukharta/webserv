#include "Config.hpp"

std::string Config::normal_split(const std::string &line, const std::string &spliter)
{
    std::string s  = line;
    ft::ltrim(s);
    ft::rtrim(s);
    s = s.substr(0 , s.find_first_of(spliter) - 1);
    return s;
}

int Config::check_args()
{
    size_t i = 0;
    
    while(i < _servers.size())
    {
        if(_servers[i].port == -1 || _servers[i].host == "NULL")
        {;return 1;}
        
        size_t j = 0;
        while(j < _servers[i].locations.size())
        {
            if(_servers[i].locations[j].path == "NULL" 
            || _servers[i].locations[j].method == "NULL" 
            || (_servers[i].locations[j].root == "NULL" && _servers[i].locations[j].redirect == "NULL")
            || _servers[i].locations[j].autoindex == "NULL")
            {   return 1;}
            j++;
        }
        i++;
    }
    return 0;
}

void Config::set_defaults(size_t i)
{
    _servers[i].port = -1;
    _servers[i].host = "NULL";
    _servers[i].index = "NULL";
    _servers[i].root = "NULL";
    // _servers[i].name = "";
    _servers[i].bodysize_limit = -1;
    _servers[i].isChild = 0;
    _servers[i].default_error_pages = "default/path";
}

void Config::set_defaults_2(Location *l)
{
    l->path = "NULL";
    l->method = "NULL";
    l->root = "NULL";
    l->autoindex = "NULL";
    l->cgi_path = "NULL";
    l->extension = "NULL";
    l->redirect = "NULL";
    l->bodysize_limit = _servers[index].bodysize_limit;
    l->upload_path = "NULL";
    l->index = _servers[index].index;
    l->default_error_pages = _servers[index].default_error_pages;
}

void Config::parse_location(std::string &line, const std::string &spliter)
 {
    ft::ltrim(line);
    ft::rtrim(line);
    std::vector<std::string> s;
    Location l;
    std::size_t prev = 0, pos;
    std::string str;


    set_defaults_2(&l);
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
                if(s[pos].find("path = ") != std::string::npos && test == "path" && l.path == "NULL")
                {
                    str = s[pos].substr(s[pos].find("path = ") + 7, s[pos].length());
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                       {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.path = str;
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'm' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("method = ") != std::string::npos && test == "method" && l.method == "NULL")
                {
                    {
                        str = s[pos].substr(s[pos].find("method = ") + 9, s[pos].length());
                        if(str == "")
                            {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                        l.method = str;
                        ////// ALLOWED METHODS
                        l.allowed_methods = ft::split_to_lines(l.method,"/");
                        // std::cout << l.method << " " << l.allowed_methods[0] << std::endl;

                    }
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'r' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("root = ") != std::string::npos && test == "root" && l.root == "NULL")
                {
                    {str = s[pos].substr(s[pos].find("root = ") + 7, s[pos].length());
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.root = str;}
                }
                else if(s[pos].find("redirect = ") != std::string::npos && test == "redirect"  && l.redirect == "NULL")
                {
                   
                    {str = s[pos].substr(s[pos].find("redirect = ") + 11, s[pos].length() - 11);
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.redirect = str;}

                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'a' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("autoindex = ") != std::string::npos && test == "autoindex"  && l.autoindex == "NULL")
                {
                    {str = s[pos].substr(s[pos].find("autoindex = ") + 12, s[pos].length());
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.autoindex = str;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'c' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("cgi_path = ") != std::string::npos && test == "cgi_path"  )
                {
                    {str = s[pos].substr(s[pos].find("cgi_path = ") + 11, s[pos].length());
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.cgi_path = str;
                    }
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'e' :{
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("extension = ") != std::string::npos && test == "extension"  )
                {
                    {str = s[pos].substr(s[pos].find("extension = ") + 12, s[pos].length());
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.extension = str;
                    }
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'b': {
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("bodysize_limit = ") != std::string::npos && test == "bodysize_limit")
                {
                   
                    {str = s[pos].substr(s[pos].find("bodysize_limit = ") + 17, s[pos].length() - 17);
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "" || ft::atoi(str.c_str()) <= 0)
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.bodysize_limit = ft::atoi(str.c_str());}

                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'i': {
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("index = ") != std::string::npos && test == "index")
                {
                    {str = s[pos].substr(s[pos].find("index = ") + 8, s[pos].length() - 8);
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.index = str;}

                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'u': {
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("upload_path = ") != std::string::npos && test == "upload_path")
                {
                    {str = s[pos].substr(s[pos].find("upload_path = ") + 14, s[pos].length() - 14);
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.upload_path = str;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                break;
            }
            case 'd': {
                std::string test = normal_split(s[pos], "=");
                if(s[pos].find("default_error_pages = ") != std::string::npos && test == "default_error_pages")
                {
                    
                    {str = s[pos].substr(s[pos].find("default_error_pages = ") + 22, s[pos].length() - 22);
                    str = str.substr(0, str.length());
                    ft::ltrim(str);
                    ft::rtrim(str);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s[pos] + "\n");}
                    l.default_error_pages = str;}
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

void Config::parse_buffer(const std::string &s)
{
    // std::cout << s << std::endl;
    Server serv;
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
            
            checker++;
            serv.name = str;
            // index_loc = 0;
            _servers.push_back(serv);
            set_defaults(index);
            
        }
        else{
            index++;
            checker--;
        }
    }
    else{
        str = s;
        ft::rtrim_space(str);
        if(str[str.length() - 1] != ';' || (str.find("location = {") != std::string::npos && str[str.length() - 2] != '}'))
        {throw std::invalid_argument( "bad argumet :" + str + "\n");}
        switch(s[0]){
            case 'h':{
                std::string test = normal_split(s, "=");
                if(s.find("host = ") != std::string::npos && test == "host" && _servers[index].host == "NULL")
                {
                    {str = s.substr(s.find("host = ") + 7, s.length() - 7);
                    str = str.substr(0, str.length() - 1);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                    _servers[index].host = str;}
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'p' :{
                std::string test = normal_split(s, "=");
                if(s.find("port = ") != std::string::npos && test == "port" && _servers[index].port == -1)
                {
                    {
                        str = s.substr(s.find("port = ") + 7, s.length() - 7);
                        str = str.substr(0, str.length() - 1);
                        if(str == "" || ft::atoi(str.c_str()) <= 0 || ft::atoi(str.c_str()) > 65353) // port range
                        {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                        _servers[index].port = ft::atoi(str.c_str());
                    }
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'd': {
                std::string test = normal_split(s, "=");
                if(s.find("default_error_pages = ") != std::string::npos && test == "default_error_pages" && _servers[index].default_error_pages == "default/path")
                {
                    
                    {str = s.substr(s.find("default_error_pages = ") + 22, s.length() - 22);
                    str = str.substr(0, str.length() - 1);
                    if(str == "")
                        {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                    _servers[index].default_error_pages = str;}
                    // std::cout << _servers[index].default_error_pages << std::endl;
                }
                 else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'b': {
                std::string test = normal_split(s, "=");
                if(s.find("bodysize_limit = ") != std::string::npos && test == "bodysize_limit" && _servers[index].bodysize_limit == -1)
                {
                   
                    {str = s.substr(s.find("bodysize_limit = ") + 17, s.length() - 17);
                    str = str.substr(0, str.length() - 1);
                    if(str == "" || ft::atoi(str.c_str()) <= 0)
                        {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                    _servers[index].bodysize_limit = ft::atoi(str.c_str());}
                // std::cout << _servers[index].bodysize_limit << std::endl;
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'i': {
                std::string test = normal_split(s, "=");
                if(s.find("index = ") != std::string::npos && test == "index" && _servers[index].index == "NULL")
                {
                   
                    {str = s.substr(s.find("index = ") + 8, s.length() - 8);
                    str = str.substr(0, str.length() - 1);
                    _servers[index].index = str;}
                    // std::cout << _servers[index].index << std::endl;
                }
                else
                {throw std::invalid_argument( "bad argumet :" + s + "\n");}
                break;
            }
            case 'l': {
                std::string test = normal_split(s, "=");
                if(s.find("location = {") != std::string::npos && test == "location")
                {
                    // if(normal_split(s, "=") == "location")
                    {str = s.substr(s.find("location = {") + 12, s.length() - 12);
                    str = str.substr(0, str.length() - 1);
                    parse_location(str, ",");}
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

Config::Config(const std::string s)
{

    // count_servers(s);
    index = 0;
    checker = 0;

    std::ifstream       file(s);
    if (file)
    {

        std::stringstream os;
        std::string buffer;

        os << file.rdbuf();

        while(std::getline(os, buffer))
        {
            ft::ltrim(buffer);
            ft::rtrim(buffer);
            if(ft::isSkippable(buffer))
                continue ;
            size_t pos = buffer.find_first_of('#');
            if(pos != std::string::npos)
                buffer = buffer.substr(0, pos);
            ft::ltrim(buffer);ft::rtrim(buffer);
            parse_buffer(buffer);
            
        }
    }

    if(checker != 0 || check_args())
    {
        // std::cout << index << " " << std::endl;
        throw std::invalid_argument( "Config file syntax is not proper\n");
    }
    // size_t i = -1;
    // std::cout << "==================>" + _servers[0].name << "<================="<< std::endl;
    //  std::cout << "==================>" + _servers[0].default_error_pages << "<================="<< std::endl;
    // while(++i < _servers[0].locations.size())
    // {
    //     // std::cout << _servers[0].locations[i].path + "  " + _servers[0].locations[i].root
    //     // + "  " + _servers[0].locations[i].cgi_path + "  " + _servers[0].locations[i].extension
    //     // + " " + _servers[0].locations[i].autoindex << " " <<_servers[0].locations[i].index << " " << _servers[0].locations[i].redirect << " " << _servers[0].locations[i].upload_path;
        
    //     // std::cout << "  " + _servers[0].locations[i].method << " ";

    //     // std::cout << get_all_locations(_servers[0])[i].bodysize_limit;
        
    // }
    size_t i=0;
    while(i < _servers.size())
    {
        size_t j = i;      
        while(++j < _servers.size())
        {
            if((_servers[i].name == _servers[j].name && _servers[i].port == _servers[j].port)
            || ((_servers[i].name == "_" || _servers[j].name == "_") && _servers[i].port == _servers[j].port))
            {
                throw std::invalid_argument( "bad argumet : cannot have same port and name or not set a name\n");
            }
            else if (_servers[i].port == _servers[j].port && _servers[i].name != _servers[j].name)
            {
                _servers[j].isChild = 1;
            }
        }
        i++;
    }
    // i = 0;
    // while(i < _servers.size())
    // {
    //     std::cout << "isChild : " << _servers[i].isChild << std::endl;
    //     i++;
    // }
    std::cout << std::endl;
}
// void Config::count_servers(const std::string &s)
//  {
//     std::ifstream       file(s);
//     std::string str;
//     server_count = 0;
//     if (file)
//     {

//         std::stringstream os;
//         std::string buffer;

//         os << file.rdbuf();
//         buffer = os.str();

//         std::stringstream a(buffer);

//         while(std::getline(a, buffer))
//         {
//             ft::ltrim(buffer);
//             ft::rtrim(buffer);
//             if(ft::isSkippable(buffer))
//                 continue ;
//             if(buffer[0] == '[')
//             {
//                 str = buffer.substr(buffer.find("[") + 1, buffer.find("]") - 1);
//                 if(str == "end"){
//                     server_count++;
//                 }
//             }
//         }
//     }
//  }

Config::~Config(){}

