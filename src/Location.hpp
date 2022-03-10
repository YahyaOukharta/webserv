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

		Location(){

		}
		Location( Location const & src );
		~Location(){}

		Location &		operator=( Location const & rhs );



};

std::ostream &			operator<<( std::ostream & o, Location const & i );

#endif /* ******************************************************** LOCATION_H */