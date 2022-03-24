#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>


int main()
{
    //needs to be filled with env variables
    std::vector<const char *> vec;
    std::string cgi_location = "/usr/local/bin/php-cgi";
    std::string req_file = "test.php";
    char *test[10];
    test[0] = (char *)cgi_location.c_str();
    test[1] = (char *)req_file.c_str();
    test[2] = NULL;
    execve(test[0], test, NULL);
    return 0;

}