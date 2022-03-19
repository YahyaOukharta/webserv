#include <unistd.h>
#include <string.h>
#include <fstream>
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