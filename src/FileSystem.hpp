#ifndef FILESYSTEM_HPP
# define FILESYSTEM_HPP

# include <iostream>
# include <string>
# include <fstream>
class FileSystem
{

	private:

	public:

		FileSystem(){}
		FileSystem( FileSystem const & src );
		~FileSystem(){}

		FileSystem &		operator=( FileSystem const & rhs );


		int fileExists(std::string path){

			
			return (0);
		}

		std::string getFileContent(std::string path){
			std::ifstream ifs(path);
  			std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
			return (content);
		}


};

std::ostream &			operator<<( std::ostream & o, FileSystem const & i );

#endif /* ****************************************************** FILESYSTEM_H */