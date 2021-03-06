#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

#include "Location.hpp"

class AutoIndex
{
	private:
		AutoIndex();

		std::string		html_file_buff;
		Location		_location;
		std::string filePath;

		void				createFile()
		{
			filePath = "/tmp/autoindex_"+ft::itoa(time(NULL));
			std::ofstream	file(filePath);

			file << html_file_buff;

			file.close();
		}

		std::string	dateOfCreation(struct timespec t)
		{
			char buff[1000];

			strftime(buff, sizeof buff, "%d-%b-%Y %H:%M", localtime(&(t.tv_sec)));

			return std::string(buff);
		}

	public:
		AutoIndex(AutoIndex const &x) {	*this = x;	}
		~AutoIndex(){};
		AutoIndex	&operator=(AutoIndex const &x)
		{
			filePath = x.filePath;
			html_file_buff = x.html_file_buff;
			_location = x._location;
			return *this;
		}


		AutoIndex(std::string path, Location loc, Request const & req)
		{
			DIR				*dp;
			struct dirent	*ep;
			_location = loc;

			if (path[path.length() - 1] != '/')
				path += '/';
			dp = opendir(path.c_str());
			html_file_buff = "<html>\n<head><title>Index of " + 
								path + "</title></head>\n<body>\n<h1>Index of " + 
								path + "</h1><hr><pre>\n";
			if (dp != NULL)
			{
				while ((ep = readdir(dp)))
				{
					struct stat	st;
					std::string	namePath(path + ep->d_name);
					std::string	name( ep->d_name);
					std::string	date;
					std::string	info;
					if (stat(namePath.c_str(), &st) < 0)
					{
						perror("Stat failed");
						break ;
					}
					bool isDir = st.st_mode & S_IFDIR;
					#ifdef LINUX
						date = dateOfCreation(st.st_ctim);
					#else
						date = dateOfCreation(st.st_birthtimespec);
					#endif
					std::string	fullName = name;
					if (name.size() > 50)
						name = name.substr(0, 46) + "...";
					std::string firstPadding = std::string(50 - name.length(), ' ');
					std::string secondPadding = std::string(30, ' ');
					info = "<a href=\"" + req.getPath() + fullName +(isDir ? "/" : "") +"\">" + name + "</a>" + firstPadding + date + secondPadding + ft::itoa(st.st_size);
					html_file_buff += "\n" + info;
				}
				html_file_buff += "\n</pre><hr></body>\n</html>";
				createFile();
				closedir(dp);
			}
			else
				perror("Couldn't open the directory");
		}

		
		std::string	const	&getBuff() const {	return html_file_buff;	}

		std::string	const &getFilePath() const
		{
			return filePath;
		}
};
