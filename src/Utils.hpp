#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split_to_lines(std::string text, std::string delimiter = std::string("\r\n"))
{
    std::vector<std::string> res;
    std::string s(text);

    size_t last = 0;
    size_t next = 0;

    while ((next = s.find(delimiter, last)) != std::string::npos) 
    {   
        res.push_back(s.substr(last, next-last));
        last = next + 1; 
    } 
    res.push_back(s.substr(last));
    return res;
}
