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
    char *test[10];
    test[0] = (char *)"/usr/local/bin/php-cgi";
    test[1] = (char *)"test.php";
    test[2] = NULL;
    execve("/usr/local/bin/php-cgi", test, NULL);
    return 0;

}