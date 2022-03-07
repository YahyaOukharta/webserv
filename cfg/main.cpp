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
    config cfg(s);
    return 0;
}