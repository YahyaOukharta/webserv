#pragma once

#include "Request.hpp"
#include "Location.hpp"
#include "MimeTypes.hpp"
// Body
// Path

class Upload
{
	private:
		std::map<std::string, std::string>		_req_headers;
		Location								_location;
		std::string								boundary;

	public:
		Upload(){}
		Upload(Upload const &u)
		{
			*this = u;
		}
		Upload(Request const &req, Location const &loc)
		{
			_req_headers = req.getHeaders();
			_location = loc;
			boundary = req.getBoundary();

			std::string	fileName = getFileName();
			std::string name = req.getPath() == "/" ? _location.getRoot() + "/" + fileName : _location.getRoot() + req.getPath() + "/" + fileName;
			createFile(name, req.getBody());
		}

		Upload 		&operator=(Upload const &u)
		{
			_req_headers = u._req_headers;
			_location = u._location;

			return *this;
		}
		~Upload(){}

		std::string		getFileName()
		{
			std::map<std::string, std::string>::iterator	it = _req_headers.find("Content-Disposition");
			// std::string										extention = trim(split_first(_req_headers["Content-Type"], '/')[1], "\n\r");
			// std::cout << "[content type] =" << trim(_req_headers["Content-Type"], " \n") << std::endl;
			std::string										extention;
			std::vector<std::string>						ret = MimeTypes::mimeToExt(trim(_req_headers["Content-Type"], " \n\r"));
			if (ret.size() > 0)
				extention = ret[0];
			std::cout << "extention = " << extention << std::endl;
			std::string										fileName = "file." + extention;
			if (it != _req_headers.end())
			{
				std::string str = it->second.erase(0, it->second.find_first_of(";") + 1);
				fileName = split_first(split_first(str, ';')[1], '=')[1];
				fileName = trim(fileName, "\"\n\r");
			}

			return fileName;
		}

		void			createFile(std::string name, std::string buff)
		{
			std::ofstream	file(name);

			file << buff;
			// std::string		content;

			// for (size_t i = 0; i < buff.length(); i++)
			// {
			// 	content += buff[i];
			// 	if (buff[i] == '-' && boundary != "" && !boundary.compare(0, boundary.length() - 1, trim(buff.substr(i, buff.find("\n", i)), "-\n\r")))
			// 	{
			// 		file << content;

			// 		break;
			// 	}
			// }

			file.close();
		}

		size_t	skip_buff(std::string buf, size_t i)
		{
			for (; buf[i] != '\r'; i++)
				;

			return (i += 2);
		}
};
