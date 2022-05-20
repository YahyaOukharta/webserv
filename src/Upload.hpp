#pragma once

#include "Request.hpp"
#include "Location.hpp"
#include "MimeTypes.hpp"
#include "Utils.hpp"
// Body
// Path

class Upload
{
	private:
		std::map<std::string, std::string>		_req_headers;
		Location								_location;
		std::string								boundary;
		Request									_req;

	public:
		Upload(){}
		Upload(Upload const &u)
		{
			*this = u;
		}
		Upload(Request const &req, Location const &loc)
		{
			_req = req;
			_req_headers = req.getHeaders();
			_location = loc;
			boundary = req.getBoundary();

			// i = 0;
			// name = "";
			// buff = req.getBody();

			//std::cout << "Transfer-Encoding = " << _req_headers["Transfer-Encoding"] << std::endl;
			createFile(req.getBody());
		}

		Upload 		&operator=(Upload const &u)
		{
			_req = u._req;
			_req_headers = u._req_headers;
			_location = u._location;
			boundary = u.boundary;

			// i = u.i;
			// name = u.name;
			// buff = u.buff;

			return *this;
		}
		~Upload() {}
		std::string		getFileName(std::string str = "")
		{
			// std::map<std::string, std::string>::iterator	it = _req_headers.find("Content-Disposition");
			// std::string										extention = trim(split_first(_req_headers["Content-Type"], '/')[1], "\n\r");
			// std::cout << "[content type] =" << trim(_req_headers["Content-Type"], " \n") << std::endl;
			std::string										extention;
			std::vector<std::string>						ret = MimeTypes::mimeToExt(trim(_req_headers["Content-Type"], " \n\r"));
			if (ret.size() > 0)
				extention = ret[0];
			//std::cout << "extention = " << extention << std::endl;
			std::string										fileName = "uploaded_" + ft::itoa(time(NULL)) + (extention.size() ? "." + extention : "");
			
			if (str.length() > 1)
			{
				//std::cout << "\nstr = " << str << std::endl;
				std::string s = str.erase(0, str.find_first_of(";") + 1);
				fileName = split_first(split_first(s, ';')[1], '=')[1];
				fileName = trim(fileName, " \"\n\r");
				if (fileName == "\"\"\r")
					fileName = "";
			}
			// else if (it != _req_headers.end())
			// {
			// 	std::string s = it->second.erase(0, it->second.find_first_of(";") + 1);
			// 	fileName = split_first(split_first(s, ';')[1], '=')[1];
			// 	fileName = trim(fileName, "\"\n\r");
			// }

			return fileName;
		}

		void			createFile(std::string const &buff)
		{
			std::string		name;
			std::ofstream	file;

			std::string		upload_path = _location.getUploadPath();

			std::vector<size_t>	indexes = _req.getIndexes();

			if(upload_path[upload_path.length() - 1] != '/')
				upload_path  = upload_path + '/';

			if (boundary == "")
			{
				name = upload_path + getFileName();
				file.open(name);
				file.write(buff.data(), buff.size());
				file.close();
			}
			else
			{
			// std::cout << "buff = " << buff.size() << " index = " << indexes.size() << std::endl;
				for (size_t i = 0; i < indexes.size() - 1; i++)
				{
					size_t	j = skip_buff(buff, indexes[i]);


					std::string str(buff.data()+j, buff.find("\n", j) - j);
					str = split_first(str, ':')[1];
					name = getFileName(str);
					if (name == "")
						continue;
					name = upload_path + name;
					
					// std::cout << "j = " << j << " name = " << name << " index = " << indexes[i] << std::endl;
					int fd = open(name.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
					j = skip_buff(buff, skip_buff(buff, skip_buff(buff, j)));

					write(fd,buff.data() + j, indexes[i + 1] - j - 31);
					close(fd);
					//file.close();
				}
			}
		}

		size_t	skip_buff(std::string const &buf, size_t i)
		{
			for (; buf[i] != '\r'; i++)
				;

			return (i += 2);
		}

		size_t	not_from_boundary(std::string str, size_t i)
		{
			for (; i < str.size(); i++)
				if (str[i] != '-' && (str[i] > '9' || str[i] < '0'))
					break;
			return i;
		}

		size_t	getFSize(std::string name)
		{
			struct stat	st;

			stat(name.c_str(), &st);

			return st.st_size;
		}
		// bool	is_done()	{
		// 	std::cout << "i = " << i << " size = " << buff.size() << std::endl;
		// 	return i >= buff.size();
		// }

		// size_t	getIndex() const {	return i;	}

};
