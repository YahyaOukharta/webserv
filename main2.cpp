// #include "src/ServerConfig.hpp"
#include "src/Server.hpp"
#include "src/FileSystem.hpp"

int main(int ac, char **av)
{
    ServerConfig conf("hello", "0.0.0.0", 8000, 10);
    Server srv(conf);

    srv.initServer();
    //srv.run();
    while(1){
        srv.async();
    }


    FileSystem fs;

    std::string buf = fs.getFileContent("www/index.html");
    std::cout << buf << std::endl;
    return (0);
}