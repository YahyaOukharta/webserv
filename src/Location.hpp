#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>

class Location
{

	public:

		Location();
		Location( Location const & src );
		~Location();

		Location &		operator=( Location const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Location const & i );

#endif /* ******************************************************** LOCATION_H */