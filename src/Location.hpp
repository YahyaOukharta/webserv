#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>

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
		Location( Location const & src );
		~Location(){}
		Location(){}

		Location &		operator=( Location const & rhs );



};

std::ostream &			operator<<( std::ostream & o, Location const & i );

#endif /* ******************************************************** LOCATION_H */