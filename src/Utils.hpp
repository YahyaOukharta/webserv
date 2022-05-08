#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <locale>
#include <stdio.h>
#include <time.h>

std::vector<std::string> split_to_lines(std::string text, std::string delimiter = std::string("\r\n"))
{
    std::vector<std::string> res;
    std::string s(text);

    size_t last = 0;
    size_t next = 0;
    std::string token;
    while ((next = s.find(delimiter, last)) != std::string::npos) 
    {   
        token = s.substr(last, next-last);
        if (token.size()|| delimiter=="\r\n\r\n")
            res.push_back(token);
        last = next + delimiter.size(); 
    } 
    token = s.substr(last);
    
    if (token.size()|| delimiter=="\r\n\r\n")
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


void trim(std::string &s){
    ltrim2(s);
    rtrim(s);
}

std::string asciitolower(std::string const &s) {
	std::string ret;
	for(size_t i = 0; i < s.size(); ++i){
		char in = s[i];
		if (in <= 'Z' && in >= 'A')
        	ret.push_back(in - ('Z' - 'z'));
		else ret.push_back(in);
	}
    return ret;
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



std::string getDate() {
  char buf[1000];
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);

  return buf;
}

// Chadi's Functions

std::vector<std::string>		split(std::string str, char delem)
{
	std::vector<std::string>					lines;
	std::stringstream	ss(str);

	std::string			s;
	while(std::getline(ss, s, delem))
		lines.push_back(s);
	
	return lines;
}

std::vector<std::string>		split_first(std::string str, char delem)
{
	std::vector<std::string>		ret;

	std::string s = str.substr(0, str.find(delem));
	ret.push_back(s);

	if (s.size() < str.size())
		ret.push_back(str.substr(s.size() + 1, str.size()));

	return ret;
}

std::string		trim(std::string str, std::string t)
{
	size_t	right = str.find_last_not_of(t);
	size_t	left = str.find_first_not_of(t);

	// Trim Right
	if (right != std::string::npos)
		str.erase(right + 1);
	
	// Trim Left
	if (left != std::string::npos)
		str.erase(0, left);

	return str;
}

#endif
