#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
 #include <fcntl.h>

class cgi{

    public:
        cgi();
		cgi(request R);
		cgi &operator=(const cgi &obj);
		~cgi();
        
    private:
        std::string output;
        
};