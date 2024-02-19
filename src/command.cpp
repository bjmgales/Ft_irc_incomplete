/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:21:03 by bgales            #+#    #+#             */
/*   Updated: 2024/02/19 14:51:50 by bgales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void  Server::addUsr(std::string request){
	std::string username = getUsr()->getNickname() + "!" +\
		request.substr(0, request.find(' '));
	for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); it++){
		if (username == (*it).getUsername()){
			const char err[] = "ERROR : Username already used.\r\n";
			send(_currUsr, err, strlen(err), 0);
			return;
		}
	}
	getUsr()->setUsername(username);
}

void  Server::addNick(std::string request){
	for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); it++){
		if (request == (*it).getNickname()){
			const char err[] = "ERROR : Nickname already used.\r\n";
			send(_currUsr, err, strlen(err), 0);
			return;
		}
	}
	getUsr()->setNickname(request);
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

void Server::joinMsg(Channel chan){
	std::string msg =  ":" + getUsr()->getUsername() + "@127.0.0.1 JOIN :" + chan.getName() + "\r\n";
	std::string topic = ":ftirc.com " + RPL_TOPIC + " " + getUsr()->getNickname()\
		+ " " + chan.getName() + " TOPIC: " + chan.getTopic() +  "\r\n";
	getUsr()->setServRep(msg);
	getUsr()->setServRep(topic);
	chanNameUpdate(&chan);
}
void  Server::joinChan(std::string request){

	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		if (request == (*it).getName()){
			if (!isDuplicatePtr((*it).getUsers(), getUsr()) \
					&& (!isDuplicatePtr((*it).getOperators(), getUsr())))
				(*it).addUsr(getUsr());
			joinMsg(*it);
			return;
		}
	}
	Channel chan(request, this, getUsr());
	_channels.push_back(chan);
	joinMsg(chan);
}

void Server::chanMsg(std::string request){

}
void  Server::privMsg(std::string request){
	if (request[0] == '#'){
		chanMsg(request);
		return;
	}
	std::string dest_usr = request.substr(0, request.find(' '));
	std::string msg = request.substr(request.find(':') + 1);

	for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); it++){
		if (dest_usr == (*it).getNickname() && (*it).getSock() > 3){
			std::string str = ":" +getUsr()->getUsername() + "@127.0.0.1 PRIVMSG " + dest_usr + " :" + msg + "\r\n";
			(*it).setServRep(str);
			if (!isDuplicatePtr(_waitUsers, &(*it)))
				_waitUsers.push_back(&(*it));
			return;
		}
	}

}

void  Server::kickUsr(std::string request){
	std::string chan_name = request.substr(0, request.find(' ') + 1);
	request = request.substr(request.find(' ') + 1);
	std::string to_kick = request.substr(0, request.find(' '));
	std::string reason = request.substr(request.find(':') + 1);
	if (reason.empty())
		reason = "undefined";
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		if ((*it).getName() == chan_name)
		{
			std::list<Client *> tmp = (*it).getUsers();
			std::cout << "Users size" << (tmp.size())<< std::endl;
			for (std::list<Client *>::iterator it2 = tmp.begin(); it2 != tmp.end(); it2++){
				if ((*it2)->getNickname() == to_kick){
					setChanRPL(":" + getUsr()->getUsername() + "@127.0.0.1 KICK "\
						+ (*it).getName() + " " + to_kick + " :" + reason + "\r\n", (*it));
					(*it).removeUser(*it2);
					chanNameUpdate(&(*it));
					return;
				}
			}
		}
	}
	std::string err = ":ft_irc.com 482 " + getUsr()->getNickname() \
	+ " " + chan_name + " :" + to_kick + " is not in the channel or is operator\n\r";
	getUsr()->setServRep(err);
	if (!isDuplicatePtr(_waitUsers, getUsr()))
			_waitUsers.push_back(getUsr());
}

void  Server::inviteUsr(std::string request){
}

void  Server::chanTopic(std::string request){
}

void  Server::chanMode(std::string request){
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

void Server::setRPL(std::string RPL, std::string str){
	std::string msg;

	msg += ":ft_irc.com " + RPL + " " + getUsr()->getNickname() + " :" + str + "\r\n";
	getUsr()->setServRep(msg);
	if (!isDuplicatePtr(_waitUsers, getUsr()))
		_waitUsers.push_back(getUsr());
}

void Server::setChanRPL(std::string str, Channel chan){
	std::list <Client *> tmp = chan.getUsers();
	for (std::list<Client *>::iterator it = tmp.begin();\
			it != tmp.end(); it++){
		Client *tmp_c = *it;
		tmp_c->setServRep(str);
		if (!isDuplicatePtr(_waitUsers, tmp_c)){
			_waitUsers.push_back(tmp_c);
		}
	}
	tmp = chan.getOperators();
	for (std::list<Client *>::iterator it = tmp.begin();\
			it != tmp.end(); it++){
		Client *tmp_o = *it;
		tmp_o->setServRep(str);
		if (!isDuplicatePtr(_waitUsers, tmp_o)){
			_waitUsers.push_back(tmp_o);
		}
	}
}
