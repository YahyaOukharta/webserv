#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "config-file.hpp"
#include "string-utils.hpp"


int main()
{
    std::string s = "../configs/default.conf";
    try{
    config cfg(s);
    }
    catch(std::exception & e)
    {
        std::cout << e.what();
    }
    return 0;
}