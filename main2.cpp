// #include "src/ServerConfig.hpp"
#include "src/Server.hpp"
#include "src/FileSystem.hpp"

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    ServerConfig conf("hello", "0.0.0.0", 8000, 10);
    Server srv(conf);

    srv.initServer();
    //srv.run();
    while(1){
        srv.async();
    }
    return (0);
}