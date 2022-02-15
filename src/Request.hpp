#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

class Request
{

	public:

		Request();
		Request( Request const & src );
		~Request();

		Request &		operator=( Request const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */