// #include "src/ServerConfig.hpp"
# include "src/Webserv.hpp"

int main(int ac, char **av)
{
    signal(SIGPIPE,SIG_IGN);
    if (ac != 2){
        std::cout << "Usage : ./webserv config_file" << std::endl;
        return (1);
    }
    ;

    try {
        Webserv srv((std::string(av[1])));
        while(1)
            srv.run();
    }
    catch(webserv_exception const &e){
        std::cerr << e.what() << std::endl;
        return (1);
    }

    return (0);
}
