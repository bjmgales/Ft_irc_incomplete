#include "Server.hpp"

void Server::setRPL(std::string RPL, std::string str){
	std::string msg;

	msg += ":ft_irc.com " + RPL + " " + getUsr()->getNickname() + " :" + str + "\r\n";
	getUsr()->setServRep(msg);
	if (!isDuplicatePtr(_waitUsers, getUsr()))
		_waitUsers.push_back(getUsr());
}

void Server::sendRPL(){
	std::vector<std::string> rep = getUsr()->getServRep();
	for (int i = 0; i < rep.size(); i++){
		std::cout << "SEND (on " << getUsr()->getNickname() << + ") =" <<  rep[i] << std::endl;
		send(_currUsr, rep[i].c_str(), rep[i].length(), 0);
	}
	for (std::list<Client *>::iterator it = _waitUsers.begin(); \
		it != _waitUsers.end() && _waitUsers.size(); it++){
		if ((*it)->getSock() == getUsr()->getSock()){
			it = _waitUsers.erase(it);
		}
	}

	FD_CLR(_currUsr, &_writefds);
	getUsr()->clearServRep();
}

void Server::setChanRPL(std::string str, Channel chan){
	std::list <Client *> tmp = chan.getUsers();
	for (std::list<Client *>::iterator it = tmp.begin();\
			it != tmp.end(); it++){
		Client *tmp_c = *it;
		tmp_c->setServRep(str);
		if (!isDuplicatePtr(_waitUsers, tmp_c))
			_waitUsers.push_back(tmp_c);
	}
	tmp = chan.getOperators();
	for (std::list<Client *>::iterator it = tmp.begin();\
			it != tmp.end(); it++){
		Client *tmp_o = *it;
		tmp_o->setServRep(str);
		if (!isDuplicatePtr(_waitUsers, tmp_o))
			_waitUsers.push_back(tmp_o);
	}
}

void Server::setChanMsgRPL(std::string str, Channel chan){
	std::list <Client *> tmp = chan.getUsers();
	for (std::list<Client *>::iterator it = tmp.begin();\
			it != tmp.end(); it++){
		Client *tmp_c = *it;
		if (*it != getUsr()){
			tmp_c->setServRep(str);
			if (!isDuplicatePtr(_waitUsers, tmp_c))
				_waitUsers.push_back(tmp_c);
		}
	}
	tmp = chan.getOperators();
	for (std::list<Client *>::iterator it = tmp.begin();\
			it != tmp.end(); it++){
		Client *tmp_o = *it;
		if (*it != getUsr()){
			tmp_o->setServRep(str);
			if (!isDuplicatePtr(_waitUsers, tmp_o))
				_waitUsers.push_back(tmp_o);
		}
	}
}

void Server::chanNameUpdate(Channel *chan){
	std::string members;
	std::list<Client *> list = chan->getOperators();
	for (std::list<Client *>::iterator it = list.begin(); it != list.end(); it++){
		members += (*it)->getNickname() + " ";
	}
	list = chan->getUsers();
	for (std::list<Client *>::iterator it = list.begin(); it != list.end(); it++){
		members += (*it)->getNickname() + " ";
	}
	members = members.substr(0, members.length() - 1);
	std::string names = ":ft_irc.com " + RPL_NAMREPLY + " " + getUsr()->getNickname() + \
		" = " + chan->getName() + ":" + members + "\r\n";
	std::string e_names = ":ft_irc.com " + RPL_ENDOFNAMES + " " + getUsr()->getNickname() + \
		" " + chan->getName() + ":End of /NAMES list." + "\r\n";
	setChanRPL(names, *chan);
	setChanRPL(e_names, *chan);
}
