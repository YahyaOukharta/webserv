#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "Utils.hpp"
# include "FileSystem.hpp"
class MimeTypes
{

	public:
		typedef std::vector<std::string> str_vec;
		typedef std::vector<std::string>::iterator str_vec_it;
		typedef std::map<std::string, str_vec> vec_map;
		typedef std::map<std::string, str_vec>::iterator vec_map_it;

		static vec_map getMimeTypes(){
			vec_map m;
			std::string fname = "./MimeTypes.list";
			std::string buf = FileSystem::getFileContent(fname);
			str_vec lines = split_to_lines(buf,"\n");
			for(str_vec_it it = lines.begin(); it != lines.end(); ++it){
				it->pop_back(); // ?
				str_vec sp = split_to_lines(*it," ");
				trim(*(sp.begin()));
				for(str_vec_it it2 = ++sp.begin(); it2 != sp.end(); ++it2)
				{
					trim(*it2);
					m[*(sp.begin())].push_back(*it2);
				}
			}
			return m;
		}
		static str_vec mimeToExt(std::string const &mimeType){
			return getMimeTypes()[mimeType];
		}
		static std::string extToMime(std::string const &ext){
			if (ext == "") return "text/plain";
			vec_map m = getMimeTypes();
			for (vec_map_it it = m.begin(); it != m.end(); ++it){
				if (std::find(it->second.begin(),it->second.end(), ext) != it->second.end())
					return it->first;
				}
			return "";
		}
		static std::string getFileExtension(std::string const &filepath){
			str_vec v = split_to_lines(filepath, "/");
			v = split_to_lines(v[v.size()-1], ".");
			if (v.size()) return v[v.size() - 1];
			else return "";
		}
	private:
		MimeTypes();
		MimeTypes( MimeTypes const & src );
		~MimeTypes();

		MimeTypes &		operator=( MimeTypes const & rhs );
};

std::ostream &			operator<<( std::ostream & o, MimeTypes const & i );

#endif /* ******************************************************* MIMETYPES_H */
