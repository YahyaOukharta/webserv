// #include "src/ServerConfig.hpp"
#include "src/Server.hpp"

int main(int ac, char **av)
{
    ServerConfig conf("hello", "0.0.0.0", 8000, 10);
    Server srv(conf);

    srv.initServer();
    //srv.run();
    srv.async();
    return (0);
}