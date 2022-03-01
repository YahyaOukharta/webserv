#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include "Request.hpp"
class StatusCodes
{

	private:
		StatusCodes(){
			init();
		}
		~StatusCodes(){}
		static std::map<int,std::string> statusCodesAndStrings; // https://w3.org/Protocols/rfc2616/rfc2616-sec10.html

	public:
		std::map<int,std::string> init(){
			std::map<int,std::string> m;
			m.insert(std::make_pair(200, "OK"));
			return m;
		}
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

		static std::string getStatusMessage(int statusCode){
			return StatusCodes::statusCodesAndStrings[statusCode];
		}
};

class Response
{
	private:

		int 		statusCode;
		std::string	statusString;


	public:

		Response(){
			std::cout << StatusCodes::getStatusMessage(StatusCodes::OK()) << std::endl;
		}
		Response( Response const & src );
		~Response(){}

		Response &		operator=( Response const & rhs );


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */

