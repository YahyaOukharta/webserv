#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include "Request.hpp"
# include "signal.h"
# include "MimeTypes.hpp"
# include "Cgi.hpp"
# include "Upload.hpp"
# include "AutoIndex.hpp"

class StatusCodes
{
	private:
		StatusCodes(){}
		~StatusCodes(){}

	public:
		//static std::map<int,std::string> statusCodesAndStrings; // https://w3.org/Protocols/rfc2616/rfc2616-sec10.html

		// 1xx Informational
		static int CONTINUE(){ return 100; }
		static int SWITCHING_PROTOCOLS(){ return 101; }

		// 2xx Succesful
		static int OK(){ return 200; }
		static int CREATED(){ return 201; }
		static int ACCEPTED(){ return 202; }
		static int NON_AUTHORITATIVE_INFORMATION(){ return 203; }
		static int NO_CONTENT(){ return 204; }
		static int RESET_CONTENT(){ return 205; }
		static int PARTIAL_CONTENT(){ return 206; }

		// 3xx Redirection
		static int MULTIPLE_CHOICES(){ return 300; }
		static int MOVED_PERMANENTLY(){ return 301; }
		static int FOUND(){ return 302; }
		static int SEE_OTHER(){ return 303; }
		static int NOT_MODIFIED(){ return 304; }
		static int USE_PROXY(){ return 305; }
		static int UNUSED(){ return 306; } // Deprecated status code, not used in http/1.1
		static int TEMPORARY_REDIRECT(){ return 307; }
		static int PERMANENT_REDIRECT(){ return 308; }

		// 4xx Client Error
		static int BAD_REQUEST(){ return 400; }
		static int UNAUTHORIZED(){ return 401; }
		static int PAYMENT_REQUIRED(){ return 402; } // For future use
		static int FORBIDDEN(){ return 403; }
		static int NOT_FOUND(){ return 404; }
		static int METHOD_NOT_ALLOWED(){ return 405; }  // Method not allowed
		static int NOT_ACCEPTABLE(){ return 406; }
		static int PROXY_AUTHENTICATION_REQUIRED(){ return 407; }
		static int REQUEST_TIMEOUT(){ return 408; }
		static int CONFLICT(){ return 409; }
		static int GONE(){ return 410; }
		static int LENGTH_REQUIRED(){ return 411; }
		static int PRECONDITION_FAILED(){ return 412; }
		static int REQUEST_ENTITIY_TOO_LARGE(){ return 413; }
		static int REQUEST_URI_TOO_LONG(){ return 414; }
		static int UNSUPPORTED_MEDIA_TYPE(){ return 415; }
		static int REQUESTED_RANGE_NOT_SATISFIABLE(){ return 416; }
		static int EXPECTATION_FAILED(){ return 417; }

		// 5xx Server Error
		static int INTERNAL_SERVER_ERROR(){ return 500; }
		static int NOT_IMPLEMENTED(){ return 501; }   // Method not implemented
		static int BAD_GATEWAY(){ return 502; }
		static int SERVICE_UNAVAILABLE(){ return 503; }
		static int GATEWAY_TIMEOUT(){ return 504; }
		static int HTTP_VERSION_NOT_SUPPORTED(){ return 504; }

};

class Implemented {
	
	public:
		static std::vector<std::string> METHODS(){
			std::vector<std::string> m;
			m.push_back("OPTIONS");
			m.push_back("GET");
			m.push_back("POST");
			m.push_back("DELETE");
			return (m);
		};

		static std::vector<std::string> CONTENT_HEADERS(){
			std::vector<std::string> m;
			m.push_back("content-encoding");
			m.push_back("content-language");
			m.push_back("content-length");
			m.push_back("content-type");
			return (m);
		};
};

class Response
{
	private:

		int 		statusCode;
		std::string	statusString;

		std::map<std::string, std::string> general_headers; // Date, Server
		std::map<std::string, std::string> representation_headers; // Content-*
		std::map<std::string, std::string> response_headers; // Transfer encoding ? Location ? Allow (when method unsupported)


		Request req;
		Server *server;
		Location const *location;

		std::string process_output_filename;
		bool isCgi;
		
		int cgiPid;
	public:


		Response(Request const &_req, Server *srv) : statusCode(0), req(_req), server(srv), location(0), isCgi(0){
			cgiPid = -1;
			statusCode = handle_system_block(); // System block checks
			//std::cout << statusCode << std::endl;
			if (statusCode) return;
			init_matching_location(); // Finding matching location
			if (!location) statusCode = StatusCodes::NOT_FOUND();
			if (statusCode) return;

			statusCode = handle_request_block(); // Request block checks
			if (statusCode) return;
	
			statusCode = handle_accept_block(); // Accept block checks
			if (statusCode) return;

			bool is_ressource_missing = handle_retrieve_block();
			//std::cout << "Ressource " << (is_ressource_missing ? "":"not ") << "missing" << std::endl;
			if(is_ressource_missing){
				// ressource missing
				statusCode = handle_retrieve_when_missing_block();
				std::cout << statusCode << std::endl;
				if(statusCode) return;

				statusCode = handle_create_block();
				if(statusCode) return;

				statusCode = handle_response_when_missing_block();
				if(statusCode) return;
			}
			else {
				// ressource not missing 
				statusCode = handle_precondition_block();
				if (statusCode) return;
				
				statusCode = handle_process_block();
				if (statusCode) return;

				statusCode = handle_response_when_not_missing_block();
				if (statusCode) return;

			}
		}
		Response(){
			statusCode = StatusCodes::GATEWAY_TIMEOUT();

		}

		Response( Response const & src ): location(src.getLocation()){
			server = src.getServer();
			req = src.getRequest();

			isCgi = src.getIsCgi();
			process_output_filename = src.getProcessOutputFilename();

			general_headers = src.getGeneralHeaders();
			representation_headers = src.getRepresentationHeaders();
			response_headers = src.getResponseHeaders();

			statusCode = src.getStatusCode();
			statusString = src.getStatusString();

			cgiPid = src.getCgiPid();
		}
		~Response(){}

		Response &		operator=( Response const & rhs );

		// BLOCKS

		int handle_system_block() {  // SYSTEM BLOCK
			if (!is_service_available()){
				return (statusCode = StatusCodes::SERVICE_UNAVAILABLE());
			}
			else if (is_uri_too_long()){
				return (statusCode = StatusCodes::REQUEST_URI_TOO_LONG());
			}
			else if (are_headers_too_large()){
				return (statusCode = StatusCodes::REQUEST_ENTITIY_TOO_LARGE());
			}
			else if (
				!is_method_implemented() ||
				!are_content_headers_implemented() ||
				!is_functionality_implemented()
			){
				return (statusCode = StatusCodes::NOT_IMPLEMENTED());
			}
			else if (!are_expect_extensions_implemented()){
				return (statusCode = StatusCodes::EXPECTATION_FAILED());
			}
			else if (!is_system_block_ok()){
				return (statusCode = StatusCodes::INTERNAL_SERVER_ERROR());
			}
			return (0);
		}

		// System block utils

		bool is_service_available(){
			return (true);
		} // 503
		bool is_uri_too_long(){
			return (false);
		} // 414
		bool are_headers_too_large(){
			return (false);
		} // 413
		bool is_method_implemented(){
			std::vector<std::string> methods = Implemented::METHODS();
			return (std::find(methods.begin(), methods.end(), req.getMethod()) != methods.end());
		} // 501
		bool are_content_headers_implemented(){
			std::map<std::string, std::string> rep_headers = req.getRepresentationHeaders();
			std::vector<std::string> impl = Implemented::CONTENT_HEADERS();
			for (std::map<std::string, std::string>::iterator it = rep_headers.begin(); it!= rep_headers.end(); ++it){
				if (std::find(impl.begin(), impl.end(), asciitolower(it->first)) == impl.end())
					return (false);
			}
			return (true);
		} // 501

		bool is_functionality_implemented(){
			return (true);
		} // 501
		bool are_expect_extensions_implemented(){
			return (true);
		} // 417

		bool is_system_block_ok(){
			return (true);
		} // 500
		//
		 // REQUEST BLOCK
		int handle_request_block() {
			if(!is_method_allowed())
				return ((statusCode = StatusCodes::METHOD_NOT_ALLOWED()));
			else if (!is_authorized())
				return ((statusCode = StatusCodes::UNAUTHORIZED()));
			else if (expects_continue()) // ???????
				return ((statusCode = StatusCodes::CONTINUE()));
			else if (has_content()){
				if (is_content_too_large())
					return ((statusCode = StatusCodes::REQUEST_ENTITIY_TOO_LARGE()));
				else if (!is_content_type_accepted()){
					return ((statusCode = StatusCodes::UNSUPPORTED_MEDIA_TYPE()));
				}
				else if (from_content()){
					return ((statusCode = StatusCodes::BAD_REQUEST()));
				}
			}
			if (is_forbidden()){
				return ((statusCode = StatusCodes::FORBIDDEN()));
			}
			else if (is_method_trace() || is_method_options()){
				return ((statusCode = StatusCodes::OK()));
			}
			else if (!is_request_block_ok()){
				return ((statusCode = StatusCodes::INTERNAL_SERVER_ERROR()));
			}
			return (0);
		}
		// Request block utils
		void init_matching_location(){
			int locIndex = server->getMatchingLocationIndex(req.getPath());
			if (locIndex == -1){
				location = 0;
			}
			else{
				Location const & loc = server->getConfig().getLocations()[locIndex];
				location = &loc;
			}
		}
		bool is_method_allowed(){
			std::vector<std::string> allowed_methods = location->getAllowedMethods();
			return (std::find(allowed_methods.begin(), allowed_methods.end(), req.getMethod()) != allowed_methods.end());
		} // 405
		bool is_authorized(){
			return true;
		} // 401
		bool expects_continue(){
			return false;
		} // 100 ?
		bool has_content(){
			return req.getRepresentationHeaders().size();
		} //
		bool is_content_too_large(){
			// use this later req.getRepresentationHeaders().at("Content-length")
			size_t max_size = location->getBodySizeLimit();
			if (!max_size) max_size = server->getConfig().getBodySizeLimit();
			if (max_size) 
				return req.getBody().size() > max_size;
			return false;
		} // 413
		bool is_content_type_accepted(){ // check Content-type if included in MimeTypes
			return true;
		} // 415
		bool from_content(){ // check if request came from same content ...
			return false;
		} // 400 ? loop ? 
		bool is_forbidden(){
			return false;
		} // 403  
		bool is_method_trace(){
			return req.getMethod() == "TRACE";
		} // 200 
		bool is_method_options(){
			return req.getMethod() == "OPTIONS";
		}// 200
		bool is_request_block_ok(){
			return true;
		} // 400



		// ACCEPT BLOCK
		int handle_accept_block() {
			if (ignore_accept_block_mismatches())
				return (0);
			if (has_accept() && !accept_matches())
				return ((statusCode = StatusCodes::NOT_ACCEPTABLE()));
			if (has_accept_language() && !accept_language_matches())
				return ((statusCode = StatusCodes::NOT_ACCEPTABLE()));
			if (has_accept_charset() && !accept_charset_matches())
				return ((statusCode = StatusCodes::NOT_ACCEPTABLE()));
			if (has_accept_encoding() && !accept_encoding_matches())
				return ((statusCode = StatusCodes::NOT_ACCEPTABLE()));
			return (0);
		}
		// Accept block utils
		bool has_accept(){
			std::map<std::string, std::string> const &accept_headers = req.getRequestHeaders();
			return accept_headers.count("Accept");
		}
		struct greater_than_qvalue
		{
			inline bool operator() (const std::string& type1, const std::string& type2)
			{
				float qvalue1 = std::stof(split_to_lines(type1, ";q=")[1]);
				float qvalue2 = std::stof(split_to_lines(type2, ";q=")[1]);
				return qvalue1 > qvalue2;
			}
		};
		std::vector<std::string> parseAcceptedTypes() const {
			std::vector<std::string> accepted_types = split_to_lines(req.getRequestHeaders().find("Accept")->second,",");
			std::vector<std::string> ret, to_sort;

			for (std::vector<std::string>::iterator it = accepted_types.begin(); it != accepted_types.end(); ++it){
				std::string const &type = *it;
				if (std::find(type.begin(), type.end(), ';') == type.end())
					ret.push_back(type);
				else  to_sort.push_back(type);
			}

			std::sort(to_sort.begin(), to_sort.end(), greater_than_qvalue());
			for (std::vector<std::string>::iterator it = to_sort.begin(); it != to_sort.end(); ++it){
				ret.push_back(split_to_lines(*it, ";")[0]);
			}
			return ret;
		}
		bool accept_matches(){ /// add subtypes 
			std::string const &resPath = getRessourcePath();
			std::string const &ext = MimeTypes::getFileExtension(resPath);
 			std::string const &mimeType = MimeTypes::extToMime(ext);

			//std::vector<std::string> accepted_types = split_to_lines(req.getRequestHeaders().find("Accept")->second,",");
			std::vector<std::string> accepted_types = parseAcceptedTypes();

			//std::cout << "ext " << ext << " mimeType " << mimeType << " resPath "<< resPath << std::endl;
			return (
				std::find(accepted_types.begin(), accepted_types.end(), mimeType) != accepted_types.end() ||
				std::find(accepted_types.begin(), accepted_types.end(), "*/*") != accepted_types.end()
				);
		}
		bool has_accept_language(){
			std::map<std::string, std::string> const &accept_headers = req.getRequestHeaders();
			return accept_headers.count("Accept-Language");
		}
		bool accept_language_matches(){
			return true;
		}
		bool has_accept_charset(){
			std::map<std::string, std::string> const &accept_headers = req.getRequestHeaders();
			return accept_headers.count("Accept-Charset");
		}
		bool accept_charset_matches(){
			return true;
		}
		bool has_accept_encoding(){
			std::map<std::string, std::string> const &accept_headers = req.getRequestHeaders();
			return accept_headers.count("Accept-Encoding");
		}
		bool accept_encoding_matches(){
			return true;
		}
		bool ignore_accept_block_mismatches(){
			return false;
		}
		
		// Requested ressource root + req
		std::string getRessourcePath()  {

			if(statusCode == StatusCodes::GATEWAY_TIMEOUT())
				return location && location->getErrorPage().size() ? location->getErrorPage() : server->getConfig().getDefaultErrorPage();
			if(process_output_filename != "") // POST HANDLED IN process() block 
			{
				isCgi=1;
				return process_output_filename;
			}
			std::string const &root = location->getRoot(); 
			std::string const &path = location->getPath(); 
			//std::cout << "root: "<< root << " reqPath: "<<req.getPath() << " locPath: "<<path << std::endl;
			std::string res = (root[root.size()-1]=='/' ? root.substr(0,root.size()-1) : root) + req.getPath().substr(path.size()+(path[path.size()-1] == '/'?-1:0));
			if (FileSystem::fileExists(res) && res[res.size()-1]!='/'){
				struct stat st;
				if (stat(res.c_str(), &st) < 0)
				{
					perror("Stat failed");
					throw webserv_exception("STAT TSETTI");
				}
				if ((st.st_mode & S_IFDIR) )
					res +='/';
			}
			if (res[res.size()-1] == '/')
			{
				std::vector<std::string> index = location ? location->getIndex() : server->getConfig().getIndex();
				std::vector<std::string>::const_iterator it = index.begin();
				for(;it != index.end(); ++it){
					if (FileSystem::fileExists(res + *it))
					{
						res.append(*it);
						break;
					}
				}
				if (res[res.size() - 1] == '/')
				{
					if( (location && location->getAutoIndex()))
						res = AutoIndex(res, *location).getFilePath();
					else
						res =  location && location->getErrorPage().size() ? location->getErrorPage() : server->getConfig().getDefaultErrorPage();
				}
			}
			else 
			{
				int done = false;

				if (!done && (statusCode >= 400 || !FileSystem::fileExists(res))){
					if (!FileSystem::fileExists(res))
						statusCode = StatusCodes::NOT_FOUND();
					res =  location && location->getErrorPage().size() ? location->getErrorPage() : server->getConfig().getDefaultErrorPage();
				}
			}

			if (statusCode && MimeTypes::getFileExtension(res) == location->getCgiExtension() && req.getMethod()=="GET"){
				isCgi = 1;
				Cgi cgi(req, server, location,  res);
				std::string filename = cgi.compile();
				cgiPid = cgi.pid;
				process_output_filename = filename;
				return filename;
			}

			return res;
		}


		// retrieve block  
		int handle_retrieve_block(){
			return missing();
		}

		bool missing(){ // ressource missing || ressource for upload || redirect 
			if (location->getRedirect() != "NULL" || (location->getUploadPath() != "NULL" && req.getMethod() == "POST") )
				return true;
			std::string const & resPath = getRessourcePath();
			std::cout << "ressource path : "<< resPath << std::endl;
			return !FileSystem::fileExists(resPath);
		}

////	START MISSING TRUE 

		//precondition block here

		// retrieve after missing block  
		int handle_retrieve_when_missing_block(){
			if (moved()){
				if (moved_permanently())
					return StatusCodes::MOVED_PERMANENTLY();
					//return StatusCodes::PERMANENT_REDIRECT();
				if (moved_temporarily())
					return StatusCodes::TEMPORARY_REDIRECT();
				if (gone_permanently())
					return StatusCodes::TEMPORARY_REDIRECT();
			}
			return 0;
		}
		bool moved(){ // ressource moved
			return location->getRedirect() != "NULL" ;
		}
		bool moved_permanently(){ // redirect  308
			return true;
		}
		bool moved_temporarily(){ // redirect 307
			return false;
		}
		bool gone_permanently(){ // redirect broken? 410
			return false;
		}


		// handle create block
		int handle_create_block(){
			if (!is_method_create())
				return statusCode = StatusCodes::NOT_FOUND();
			if (!create_path())
				return statusCode = StatusCodes::INTERNAL_SERVER_ERROR(); // upload not allowed on route
			if (!create())
				return statusCode = StatusCodes::INTERNAL_SERVER_ERROR(); // couldnt upload
			return 0;
		}
		bool is_method_create(){ // is method post  404 // auto index ?
			return req.getMethod() == "POST";
		}
		bool create_path(){ // upload path defined, else 500

			return true;
		}
		bool create(){ // here process upload, 500 if fails
			std::cout << "CREATE\n";
			Upload	up(req, *location);
			statusCode = StatusCodes::CREATED();
			// return false;
			return true;
		}

		// handle response after missing block 

		int handle_response_when_missing_block(){
			if (!is_create_done())
				return statusCode = StatusCodes::ACCEPTED();
			if (create_see_other())
				return statusCode = StatusCodes::SEE_OTHER();
			else
				return statusCode = StatusCodes::CREATED();
			// END OF DIAGRAM FOR MISSING
		}
		bool is_create_done(){ // 202 accepted
			return true;
		}
		bool create_see_other(){ // 201 created, 303 if else 
			return false;
		}
////	END MISSING TRUE
////	START MISSING FALSE

		// handle precondition block 
		int handle_precondition_block(){

			return (0);
		}
		bool has_if_match(){
			return (0);
		}
		bool if_match_matches(){ // 412
			return (0);
		}
		bool has_if_unmodified_since(){
			return (0);
		}
		bool if_unmodified_since_matches(){ // 412
			return (0);
		}
		bool has_if_none_match(){
			return (0);
		}
		bool if_none_match_matches(){
			return (0);
		}
		bool has_if_modified_since(){
			return (0);
		}
		bool if_modified_since_matches(){
			return (0);
		}
		bool is_precondition_safe(){ // true 304 / false 412
			return (0);
		}

		// handle process block
		int handle_process_block(){
			if(is_method_head_get())
				return (0);
			if (is_method_delete() && !process_delete())
				return statusCode = StatusCodes::INTERNAL_SERVER_ERROR();
			else{
				if (!is_method_process())
					return statusCode = StatusCodes::INTERNAL_SERVER_ERROR();
				if (process_has_conflict())
					return statusCode = StatusCodes::CONFLICT();
				if (!process()) // 
					return statusCode = StatusCodes::INTERNAL_SERVER_ERROR();
			}
			return (0);
		}
		bool is_method_head_get(){
			return req.getMethod() == "GET";
		}
		bool is_method_delete(){
			return req.getMethod() == "DELETE";
		}
		bool process_delete(){ // 500
			return (0);
		}
		bool is_method_put(){
			return (0);
		}
		bool process_partial_put(){
			 // 400
			 return (0);
		}
		bool is_method_process(){ // 500
			return req.getMethod() == "POST";
		}
		bool process_has_conflict(){ // 409
			return (0);
		}
		bool process(){ // 500
			std::string res = getRessourcePath();
			if(MimeTypes::getFileExtension(res) == location->getCgiExtension()){
				isCgi = 1;
				Cgi cgi(req, server, location,  res);
				std::string filename = cgi.compile();
				if (filename != "")
				{
					cgiPid = cgi.pid;
					process_output_filename = filename;
					return (1);
				}
			}
			return (0);
		}

		//handle response block
		int handle_response_when_not_missing_block(){
			if (!is_method_head_get() && !is_process_done())
				return statusCode = StatusCodes::INTERNAL_SERVER_ERROR();
			if(see_other())
				return statusCode = StatusCodes::SEE_OTHER();
			if(has_multiple_choices())
				return statusCode = StatusCodes::MULTIPLE_CHOICES();
			if(to_content())
				return statusCode = StatusCodes::OK();
			else
				return statusCode = StatusCodes::NO_CONTENT();
			return (0);
		}
		bool is_process_done(){ // 202
			return (1);
		}
		bool see_other(){ // 303
			return (0);
		}
		bool has_multiple_choices(){ //300
			return (0);
		}
		bool to_content(){ //true 200 / false 204 
			return (1);
		}

////	END MISSING FALSE

		//HEADERS
		void initGeneralHeaders(){
			general_headers.clear();
			general_headers.insert(std::pair<std::string,std::string>("Date",getDate()));
			general_headers.insert(std::pair<std::string,std::string>("Server","Webserv"));
		}

		int initRepresentationHeaders(){//// normal file, missing file, cgi, autoindex 
			std::string resPath;

			resPath = getRessourcePath();
			// std::cout << "final res path " << resPath << std::endl;

			if(isCgi || process_output_filename!="")
			{
				int status;
				int ret = waitpid(cgiPid,&status, WNOHANG);
				if (ret != cgiPid)
					return 1;
				//int nbytes;
				char cgi_buff[1024] = {0};
				// //to reset cursor to first byte of the file
				int fd = open(resPath.c_str(), O_RDWR , 0777);
				std::string cgi_output;
				int n;
				while((n = read(fd, cgi_buff, 1024)) > 0){
					cgi_output.append(cgi_buff, n);
				}
				if (cgi_output.find("Status: ")!= std::string::npos){
					statusCode = ft::atoi(cgi_output.c_str() + cgi_output.find("Status: ") + 8);
				}
			}

			representation_headers.clear();
			if(!isCgi)
				representation_headers.insert(std::pair<std::string,std::string>("Content-Type",MimeTypes::extToMime(MimeTypes::getFileExtension(resPath))));
			representation_headers.insert(std::pair<std::string,std::string>("Content-Length", std::to_string(FileSystem::getFileSize(resPath))));
			
			//representation_headers.insert(std::pair<std::string,std::string>("Transfer-Encoding","chunked"));
			return (0);
		}

		void initResponseHeaders(){
			response_headers.clear();

			if(statusCode == StatusCodes::MOVED_PERMANENTLY())
				response_headers.insert(std::pair<std::string,std::string>("Location",location->getRedirect()));
			// response_headers.insert(std::pair<std::string,std::string>("Content-Length","Webserv"));
		}

		// GETTERS
		std::string getProcessOutputFilename()const{
			return process_output_filename;
		}
		bool getIsCgi() const{
			return isCgi;
		}
		Location const*getLocation()const {return location;}
		Server *getServer() const {return server;}
		Request getRequest() const {return req;}

		std::map<std::string, std::string> getGeneralHeaders() const{
			return general_headers;
		}
		std::map<std::string, std::string> getRepresentationHeaders() const{
			return representation_headers;
		}
		std::map<std::string, std::string> getResponseHeaders() const{
			return response_headers;
		}
		int getStatusCode()const {return statusCode;}
		std::string getStatusString() const {return statusString;}
		int getCgiPid()const {return cgiPid;}

		std::string getResponseBufferWithoutBody() {

		
			if (initRepresentationHeaders())return "";
			initResponseHeaders();
			initGeneralHeaders();
			std::string res = "";
			std::string nl = "\r\n";

			res.append("HTTP/1.1 ");
			res.append(std::to_string(statusCode));
			res.append(nl);

			std::map<std::string, std::string>::const_iterator it = general_headers.begin();
			for(;it != general_headers.end(); ++it){
				res.append(it->first + ": " + it->second + nl);
			}
			it = representation_headers.begin();
			for(;it != representation_headers.end(); ++it){
				res.append(it->first + ": " + it->second + nl);
			}
			it = response_headers.begin();
			for(;it != response_headers.end(); ++it){
				res.append(it->first + ": " + it->second + nl);
			}
			if(!(isCgi))
			{	
				res.append(nl);
			}
			return res;
		}

		void timeout(){
			statusCode = StatusCodes::GATEWAY_TIMEOUT();
			isCgi = false;
			process_output_filename = "";
			kill(cgiPid, SIGKILL);
		}

};


std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */
