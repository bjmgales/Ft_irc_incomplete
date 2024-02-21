/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:21:03 by bgales            #+#    #+#             */
/*   Updated: 2024/02/21 14:48:59 by bgales           ###   ########.fr       */
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

void  Server::privMsg(std::string request){
	if (request[0] == '#'){
		chanMsg(request);
		return;
	}
	std::string dest_usr = request.substr(0, request.find(' '));
	std::string msg = request.substr(request.find(':') + 1);

	for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); it++){
		if (dest_usr == (*it).getNickname() && (*it).getSock() > 3){
			std::string str = ":" + getUsr()->getUsername() + "@127.0.0.1 PRIVMSG " + dest_usr + " :" + msg + "\r\n";
			(*it).setServRep(str);
			if (!isDuplicatePtr(_waitUsers, &(*it)))
				_waitUsers.push_back(&(*it));
			return;
		}
	}
	std::string err = ":ft_irc.com 401 " + getUsr()->getNickname() \
		+ " " + dest_usr + " : No such nickname.\r\n";
	getUsr()->setServRep(err);
	if (!isDuplicatePtr(_waitUsers, getUsr()))
		_waitUsers.push_back(getUsr());
}

bool	Server::isChanMember(Channel chan){

	if (!isDuplicatePtr(chan.getOperators(), getUsr()) && \
			!isDuplicatePtr(chan.getUsers(), getUsr())){
		std::string err = ":ft_irc.com 401 " + getUsr()->getNickname() \
			+ " " + chan.getName() + ": Join channel first.\r\n";
		getUsr()->setServRep(err);
		if (!isDuplicatePtr(_waitUsers, getUsr()))
			_waitUsers.push_back(getUsr());
		return (false);
	}
	return true;
}

bool	Server::isChanOperator(Channel chan){

	if (!isDuplicatePtr(chan.getOperators(), getUsr())){
		std::string err = ":ft_irc.com 401 " + getUsr()->getNickname() \
			+ " " + chan.getName() + ": Command was not performed." \
				" You are not a channel operator.\r\n";
		getUsr()->setServRep(err);
		if (!isDuplicatePtr(_waitUsers, getUsr()))
			_waitUsers.push_back(getUsr());
		return (false);
	}
	return true;
}
