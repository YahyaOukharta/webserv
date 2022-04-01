// #include "src/ServerConfig.hpp"
# include "src/Webserv.hpp"

# include "src/MimeTypes.hpp"
# include "src/Config.hpp"

int main(int ac, char **av)
{
    // ServerConfig conf("hello", "0.0.0.0", 8000, 10);
    // Server srv(conf);

    // srv.initServer();
    // //srv.run();
    // while(1){
    //     srv.async();
    // }
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

    // for(std::vector<Config::Server>::iterator it = cfg._servers.begin(); it != cfg._servers.end(); ++it){
    //     Server *srv = new Server(*it);
    //     srv->initServer();
    // }
    return (0);
}
