#include "Server.hpp"
#include <errno.h>
#include <string.h>
int main(int argc, char **argv){
	Server serv(std::stoi(argv[1]), argv[2]);

	if (serv.getSock() < 0){
		std::cerr << "Error: Server could not start."<< std::endl;
		return (-1);
	}
	while (1){
		if (serv.selectMonitoring() < 0){
			std::cout <<"Error: Select failure." << std::endl;
			exit (1);
		}
		else{
			serv.sockReady();
		}
	}
}

