#include "src/AutoIndex.hpp"

int main(int ac, char **av)
{
	if (ac > 1)
	{
		AutoIndex ind(av[1]);

		std::cout << ind.getBuff() << std::endl;
	}

	return 0;
}
