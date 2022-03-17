#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include "Request.hpp"

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

		// 5xx Client Error
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

		std::map<std::string, std::string> general_headers;
		std::map<std::string, std::string> representation_headers;
		std::map<std::string, std::string> response_headers;

		std::string body;

		Request req;
		Server *server;
		Location const *location;
	public:

		Response(Request const &_req, Server *srv) : statusCode(0), req(_req), server(srv){
			int status = 0;
			status = handle_system_block(); // System block checks
			std::cout << statusCode << std::endl;
			if (status) return;

			init_matching_location(); // Finding matching location
			if (!location) status = StatusCodes::NOT_FOUND();
			if (status) return;

			status = handle_request_block(); // Request block checks
			std::cout << statusCode << std::endl;
			if (status) return;

		}
		Response(){}
		Response( Response const & src );
		~Response(){}

		Response &		operator=( Response const & rhs );

		// BLOCKS
		bool handle_system_block() {  // SYSTEM BLOCK
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



		bool handle_request_block() { // REQUEST BLOCK
			if(!is_method_allowed())
				return ((statusCode = StatusCodes::METHOD_NOT_ALLOWED()));
			else if (!is_authorized())
				return ((statusCode = StatusCodes::UNAUTHORIZED()));
			else if (expects_continue()) // ???????
				return ((statusCode = StatusCodes::CONTINUE()));
			else if (has_content()){
				if (is_content_too_large())
					return ((statusCode = StatusCodes::REQUEST_ENTITIY_TOO_LARGE()));
				else if (is_content_type_accepted()){
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
			else if (is_request_block_ok()){
				return ((statusCode = StatusCodes::INTERNAL_SERVER_ERROR()));
			}
			return (0);
		}
		bool handle_accept_block() {

			return (0);
		}
		bool handle_retrieve_block() {

			return (0);
		}
		bool handle_precondition_block() {

			return (0);
		}
		bool handle_create_process_block() {

			return (0);
		}
		bool handle_response_block() {

			return (0);
		}
		bool handle_alternative_block() {

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
		int is_method_allowed(){
			std::vector<std::string> allowed_methods = location->getAllowedMethods();
			return (std::find(allowed_methods.begin(), allowed_methods.end(), req.getMethod()) != allowed_methods.end());
		} // 405
		int is_authorized(){
			return true;
		} // 401
		int expects_continue(){
			return false;
		} // 100 ?
		int has_content(){
			return req.getRepresentationHeaders().size();
		} //
		int is_content_too_large(); // 413
		int is_content_type_accepted(); // 415
		int from_content(); // 400 ? loop ? 
		int is_forbidden(); // 403  
		int is_method_trace(); // 200 
		int is_method_options(); // 200
		int is_request_block_ok(); // 400

		//



		
};


std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */
