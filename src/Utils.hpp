#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <locale>

std::vector<std::string> split_to_lines(std::string text, std::string delimiter = std::string("\r\n"))
{
    std::vector<std::string> res;
    std::string s(text);

    size_t last = 0;
    size_t next = 0;

    while ((next = s.find(delimiter, last)) != std::string::npos) 
    {   
        res.push_back(s.substr(last, next-last));
        last = next + delimiter.size(); 
    } 
    res.push_back(s.substr(last));
    return res;
}

bool notisspace(unsigned char ch){
    return !std::isspace(ch);
}
static inline std::string ltrim(std::string s) {

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notisspace));
    return s;
}
static void ltrim2(std::string &s)
{
    s.erase(0, s.find_first_not_of("\t "));
}
 
static void rtrim(std::string &s)
{
    s.erase(s.find_last_not_of("\t ") + 1);
}


static void trim(std::string &s){
    ltrim2(s);
    rtrim(s);
}



// to do : add here all error messages/codes
class webserv_exception : public std::exception
{
    public:
    std::string _msg;
    webserv_exception(std::string msg) : _msg(msg){}
    virtual ~webserv_exception() throw (){};
    const char * what () const throw () {
        return _msg.c_str();
   }
};

#endif
