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

			//std::cout << "Transfer-Encoding = " << _req_headers["Transfer-Encoding"] << std::endl;
			createFile(req.getBody());
		}

		Upload 		&operator=(Upload const &u)
		{
			_req_headers = u._req_headers;
			_location = u._location;

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

		void			createFile(std::string buff)
		{
			std::string		name;
			std::ofstream	file;
			std::string		content;
			std::string		upload_path = _location.getUploadPath();

			size_t			i = 500;

			content.reserve(buff.size());

			if(upload_path[upload_path.length() - 1] != '/')
				upload_path  = upload_path + '/';
			
			// For chunked encoding

			bool			chunked = _req_headers["Transfer-Encoding"] == "chunked";
			// unsigned int	chunk_size;

			
			// if (chunked)
			// {
			// 	std::string		hex = buff.substr(i, skip_buff(buff, i));
			// 	i = skip_buff(buff, i);
			// 	chunk_size = std::stol(hex, nullptr, 16) + i;
			// 	std::cout << "NUM = |" << buff[chunk_size] << "|" << std::endl;
			// 	// std::cout << "buff\n" << buff[skip_buff(buff, i) + chunk_size] << std::endl;
			// }

			// Checking boundary for multipart form data

			if (buff[i] == '-' && boundary != "" && boundary == trim(buff.substr(i, not_from_boundary(buff, i) - i), "-\n\r"))
			{
				i = skip_buff(buff, i);
				std::string str = buff.substr(i, buff.find("\n", i) - i);
				name = getFileName(split_first(str, ':')[1]);
				name = upload_path + name;
				file.open(name);
				// std::cout << (file.is_open() ? "YES ITS OPEN" : "NO ITS NOT OPEN") << std::endl;
				i = skip_buff(buff, i);
				i = skip_buff(buff, skip_buff(buff, i));
			}
			else	// Thers is no boundary
			{
				name = getFileName();		
				file.open(upload_path + name);
			}

			for (; i < buff.length(); i++)
			{
				// Check if there is a boundary

				if (buff[i] == '-' && boundary != "" && boundary == trim(buff.substr(i, not_from_boundary(buff, i) - i), "-\n\r"))
				{
					file.write(content.data(), content.size());
					file.close();
					i = skip_buff(buff, i);

					std::string str = buff.substr(i, buff.find("\n", i) - i);
					if (str.length() < 1)
						return ;
					str = split_first(str, ':')[1];
					name = getFileName(str);
					// if (name == "")
					// 	return ;

					name = upload_path + name;

					// break;
					content = "";
					content.reserve(buff.size());

					file.open(name);
					i = skip_buff(buff, i);
					i = skip_buff(buff, skip_buff(buff, i));
				}
				size_t next_bound = 0;
				if (boundary != "")
				{	
					next_bound = buff.find(boundary, i);
					content.assign(buff.data() + i, next_bound - 29 - i);
					i = next_bound - 29 ;
				}
				else if (chunked)
				{
					std::string		hex = buff.substr(i, skip_buff(buff, i));
					std::cout << "HEX = " << hex << std::endl;
					unsigned int	chunk_size = std::stol(hex, nullptr, 16);
					// std::cout << "chunk_size = " << chunk_size << std::endl;
					if (!chunk_size)
						break;
					i = skip_buff(buff, i);
					content.append(buff.data() + i, chunk_size);
					i += chunk_size;
				}
				else
				{
					content = buff;
					i = buff.size();
				}
			}
			file.write(content.data(), content.size());
			file.close();
		}

		size_t	skip_buff(std::string buf, size_t i)
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
};
