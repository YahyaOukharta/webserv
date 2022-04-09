#ifndef FILESYSTEM_HPP
# define FILESYSTEM_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <cstring>
# include <sys/stat.h>
# define BUFFER_SIZE 1024
class FileSystem
{
	private:

	public:

		FileSystem(){}
		FileSystem( FileSystem const & src );
		~FileSystem(){}

		FileSystem &		operator=( FileSystem const & rhs );


		static bool fileExists(std::string const &path){

			struct stat buffer;   
			return (stat(path.c_str(), &buffer) == 0);
		}

		static std::string getFileContent(std::string const &path){
			std::ifstream ifs(path.c_str());
  			std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
			return (content);
		}

		static std::string getFileContent(int fd){
			std::string buf;
			char line[BUFFER_SIZE + 1]={0};
			int ret;
			while((ret = read(fd, line, BUFFER_SIZE)) > 0)
			{				
				buf.append(line);
				memset(line,0, BUFFER_SIZE);
			}
			return (buf);
		}


};

std::ostream &			operator<<( std::ostream & o, FileSystem const & i );

#endif /* ****************************************************** FILESYSTEM_H */
