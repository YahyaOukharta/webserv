#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "config-file.hpp"
#include "string-utils.hpp"


int main()
{
    std::string s = "../configs/conf.conf";
    config cfg(s);
    // cfg.print_config();
    return 0;
}