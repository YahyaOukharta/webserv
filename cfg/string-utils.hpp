#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

class ft{
    public:
    // const std::string WHITESPACE = " \n\r\t\f\v";
 
static void ltrim(std::string &s,  const std::string& chars = "\t ")
{
    s.erase(0, s.find_first_not_of(chars));
}
 
static void rtrim(std::string &s, const std::string& chars = "\t ")
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
	long long	n;
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