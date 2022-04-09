#ifndef PARSE_UTILS_HPP
# define PARSE_UTILS_HPP
#include <iostream>
class ft {
    public:
    // const std::string WHITESPACE = " \n\r\t\f\v";
        static std::vector<std::string> split_to_lines(std::string text, std::string delimiter = std::string("\r\n"))
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
        static void ltrim_space(std::string &s,  const std::string& chars = " \t")
        {
            s.erase(0, s.find_first_not_of(chars));
        }
        
        static void rtrim_space(std::string &s, const std::string& chars = " \t")
        {
            s.erase(s.find_last_not_of(chars) + 1);
        }

        static void ltrim(std::string &s,  const std::string& chars = "\t {}")
        {
            s.erase(0, s.find_first_not_of(chars));
        }
        
        static void rtrim(std::string &s, const std::string& chars = "\t {}")
        {
            s.erase(s.find_last_not_of(chars) + 1);
        }

        //to find out if the line is irrelivent "comment"
        static bool isSkippable(std::string s)
        {
            return (s.size() == 0 || s[0] == '#');
        }
        static int	atoi(const char *str)
        {
            long	n;
            int			sign;
            int			i;

            i = 0;
            sign = 1;
            n = 0;
            while (str[i] != '\0' && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
                i++;
            if (str[i] == '+' || str[i] == '-')
            {
                if (str[i++] == '-')
                    sign = -1;
            }
            while (str[i] >= '0' && str[i] <= '9')
            {
                n = n * 10;
                n = n + str[i] - '0';
                i++;
            }
            if (n < 0 && sign > 0)
                return (-1);
            if (n < 0 && sign < 0)
                return (0);
            return (n * sign);
        }
};

#endif
