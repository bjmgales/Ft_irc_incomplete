#include "Server.hpp"

std::string Server::getCmd(std::string *request){
	std::string command;
	if (request->find(' ') != std::string::npos){
		command = request->substr(0, request->find(' '));
		*request = request->substr(request->find(' ') + 1);
	}

	cleanCarriageRet(request);
	return (command);
}

void Server::treatCmd(std::string command, std::string request){
		if (command == "USER")
			addUsr(request);
		else if (command == "NICK")
			addNick(request);
		else if (command == "JOIN")
			joinChan(request);
		else if (command == "PRIVMSG")
			privMsg(request);
		else if (command == "KICK")
			kickUsr(request);
		else if (command == "INVITE")
			inviteUsr(request);
		else if (command == "TOPIC")
			chanTopic(request);
		else if (command == "MODE")
			chanMode(request);
		else if(command == "PASS")
			return;
		else
			std::cout << "Error: command \"" << command << "\" unrecognized" << std::endl;
}

void Server::handleRequest(std::string buffer){
	std::vector<std::string> request;
	std::vector<std::string> command;
	std::stringstream ss(buffer);
	std::string tmp;

	cleanCarriageRet(&buffer);
	std::cout << "Received: " << buffer << std::endl;
	if (buffer.empty()){
		std::cout << "Expecting : PASS NICK USER" << std::endl;
		return;
	}
	while(std::getline(ss, tmp, '\n')){
			request.push_back(tmp);
	}
	if (request[0] == "CAP LS\r")
		request.erase(request.begin());
	if (request.empty())
		return;
	for (int i = 0; i < request.size(); i++)
		command.push_back(getCmd(&request[i]));
	for (int i = 0; i < command.size(); i++){
		std::cout << "command =" << command[i] << "| request=" << request[i] << std::endl;
	}
	isAuthentified(command, request);
}
