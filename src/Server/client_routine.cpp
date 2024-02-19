#include "Server.hpp"

void Server::acceptNewClient() {
    Client newClient;

    newClient.setSock(accept(getSock(), newClient.getInfo(), newClient.getAddrSize()));
    if (newClient.getSock() < 0) {
        std::cout << "Error: Accept error on client socket." << std::endl;
        return;
    } else {
        newClient.setSigned(false);
        _users.push_back(newClient);
        FD_SET(newClient.getSock(), &_readfds);
        std::cout << "Connection succeeded on socket number " << newClient.getSock() << std::endl;
    }

    for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if ((*it).getSock() > _maxfd)
            _maxfd = (*it).getSock();
    }
}


void Server::clientDisconnect(int i, std::string reason){
	for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); it++){
		if ((*it).getSock() == i)
			(*it).setSock(0);
	}
	FD_CLR(i, &_readfds);
	close (i);
	std::cout << reason + " " << i << std::endl;
}


void Server::newUser(std::vector<std::string> command, std::vector<std::string> request){
	for (int i = 0; i < command.size(); i++){
		if (command[i] == "USER")
			addUsr(request[i]);
		else if (command[i] == "NICK")
			addNick(request[i]);
	std::cout << getUsr()->getNickname() << std::endl;;
	}
	if (!getUsr()->getNickname().empty() && !getUsr()->getUsername().empty()){
		getUsr()->setSigned(true);
		setRPL(RPL_WELCOME, ("Welcome to ft_irc server !" + getUsr()->getNickname()));
		setRPL(RPL_YOURHOST, ("Your host is 127.0.0.1, running version 0.42"));
		setRPL(RPL_CREATED, "This server was created moments ago.");
		setRPL(RPL_MYINFO, "ft_irc v:0.42");
		setRPL(RPL_MOTDSTART, "ft_irc Bonjour à tous et bienvenue !");
		setRPL(RPL_MOTD, "ft_irc Le diction du jour : \"Pierre qui moule n'ammasse pas rousse\".");
		setRPL(RPL_ENDOFMOTD, "End of welcome message");
	}
	else{
		const char err[] = " ERROR : need both username and nickname\r\n";
		send(_currUsr, err, strlen(err), 0);
	}
}

void Server::isAuthentified(std::vector<std::string> command, std::vector<std::string> request){

	if (getUsr()->isPass() == false){
		for (int i = 0; i < request.size(); i++){
			if (command[i] == "PASS" && request[i] == _password){
				getUsr()->setPass(true);
				isAuthentified(command, request);
				return;
			}

		}
		std::string err = "ERROR : awaiting server password.\r\n";
		send(_currUsr, err.c_str(), err.length(), 0);
	}
	else{
		if (getUsr()->getSigned() == false)
			newUser(command, request);
		else
			for (int i = 0; i < command.size(); i++)
				treatCmd(command[i], request[i]);
	}

}
