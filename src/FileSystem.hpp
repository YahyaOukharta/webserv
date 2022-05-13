#ifndef FILESYSTEM_HPP
# define FILESYSTEM_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <cstring>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
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
			// std::ifstream ifs(path.c_str());
  			// std::string content(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
			// return (content);
			int fd = open(path.c_str(), O_RDONLY);
			return getFileContent(fd);
		}

		static std::string getFileContent(int fd){
			std::string buf;
			char line[BUFFER_SIZE + 1]={0};
			int ret;
			while((ret = read(fd, line, BUFFER_SIZE)) > 0)
			{				
				buf.append(line, ret);
				memset(line,0, BUFFER_SIZE);
			}
			close(fd);
			return (buf);
		}
		static size_t getFileSize(std::string filename) // path to file
		{
			FILE *p_file = NULL;
			p_file = fopen(filename.c_str(),"rb");
			fseek(p_file,0,SEEK_END);
			size_t size = ftell(p_file);
			fclose(p_file);
			return size;
		}

};

std::ostream &			operator<<( std::ostream & o, FileSystem const & i );

#endif /* ****************************************************** FILESYSTEM_H */
