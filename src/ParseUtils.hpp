#ifndef PARSE_UTILS_HPP
# define PARSE_UTILS_HPP
#include <iostream>
#include <cstring>
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
        static int	atoi(std::string const &str){
            return atoi(str.c_str());
        }
        static int	count_digits(long long n)
{
	int	res;

	if (n == 0)
		return (1);
	res = 0;
	while (n > 0)
	{
		n /= 10;
		res++;
	}
	return (res);
}

static char		*ft_strrev(char *str)
{
	int		i;
	int		j;
	char	tmp;

	if (str)
	{
		i = 0;
		j = strlen(str) - 1;
		while (i < j)
		{
			tmp = str[i];
			str[i] = str[j];
			str[j] = tmp;
			i++;
			j--;
		}
	}
	return (str);
}

static void		ft_store(char *s, long n, size_t ndigits)
{
	size_t i;

	i = 0;
	while (i < ndigits)
	{
		s[i] = n % 10 + '0';
		n /= 10;
		i++;
	}
	ft::ft_strrev(s);
}

static std::string itoa(long nbr)
{
	long long	n;
	char		*result;
	int			negative;
	int			n_digits;

	n = nbr;
	negative = 0;
	if (nbr < 0)
	{
		n = (long)nbr * -1;
		negative = 1;
	}
	n_digits = ft::count_digits(n);
	result = (char *)malloc(sizeof(char) * (negative + n_digits + 1));
	if (!result)
		return (0);
	if (negative)
		result[0] = '-';
	result[n_digits + negative] = '\0';
	ft::ft_store(result + negative, n, n_digits);
	return (std::string(result));
}
};

#endif
