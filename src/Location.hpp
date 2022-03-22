#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

class Location
{
	private:
		std::string _path;

		std::string _root;
		int _body_size_limit;
		std::vector<std::string> _allowed_methods;

		int _auto_index; // default off
		std::string _error_page;
	public:
		struct greater_than_path
		{
			inline bool operator() (const Location& loc1, const Location& loc2)
			{
				//return (loc1.getPath() < loc2.getPath());
				std::string p1 = loc1.getPath();
				int c1 = std::count(loc1.getPath().begin(), loc1.getPath().end(), '/');
				std::string p2 = loc2.getPath();
				int c2 = std::count(loc2.getPath().begin(), loc2.getPath().end(), '/');
				return c1 > c2;
			}
		};

		Location(
			std::string path,
			std::string root,
			std::vector<std::string> allowed_methods,
			int body_size_limit,
			int auto_index,
			std::string error_page
		){
			_path = path;
			_root = root;
			_allowed_methods = allowed_methods;
			_body_size_limit = body_size_limit;
			_auto_index = auto_index;
			_error_page = error_page;
	
		}
		Location( Location const & src ){
			*this = src;
		}
		~Location(){}
		Location(){}

		Location &		operator=( Location const & rhs ){
			_path = rhs.getPath();
			_root = rhs.getRoot();
			_allowed_methods = rhs.getAllowedMethods();
			_body_size_limit = rhs.getBodySizeLimit();
			_auto_index = rhs.getAutoIndex();
			_error_page = rhs.getErrorPage();
			return *this;
		}

		std::string const &getPath()const {
			return _path;
		}
		std::string const &getRoot()const {
			return _root;
		}
		int getBodySizeLimit() const{
			return _body_size_limit;
		}
		bool getAutoIndex() const{
			return _auto_index;
		}
		std::vector<std::string> const &getAllowedMethods()const{
			return _allowed_methods;
		}
		std::string const &getErrorPage()const {
			return _error_page;
		}
};

std::ostream &			operator<<( std::ostream & o, Location const & i );

#endif /* ******************************************************** LOCATION_H */
