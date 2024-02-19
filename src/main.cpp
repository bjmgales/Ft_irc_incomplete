#include "Server.hpp"
#include <errno.h>
#include <string.h>


int main(int argc, char **argv){

	if (argc != 3){
		std::cerr << "Error: usage: \"./irc password port\"."<< std::endl;
		return (-1);
	}
	if (!isAlnum(argv[1]) || !isDigit(argv[2]))
		return (-1);

	Server serv(std::stoi(argv[2]), argv[1]);

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

