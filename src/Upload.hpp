#pragma once

#include "Request.hpp"
// Body
// Path

class Upload
{
	private:
		std::map<std::string, std::string>		_req_headers;

	public:
		Upload(){}
		Upload(Upload const &u)
		{
			*this = u;
		}
		Upload(Request const &req)
		{

		}

		Upload 		&operator=(Upload const &u)
		{
			_req_headers = u._req_headers;

			return *this;
		}
		~Upload(){}
};