#include "Server.hpp"

void Server::joinMsg(Channel chan){
	std::string msg =  ":" + getUsr()->getUsername() + "@127.0.0.1 JOIN :"\
						 	+ chan.getName() + "\r\n";
	std::string topic = ":ftirc.com " + RPL_TOPIC + " " + getUsr()->getNickname()\
							+ " " + chan.getName() + ":" + chan.getName() + \
								"topic is \"" + chan.getTopic() + "\"\r\n";
	getUsr()->setServRep(msg);
	getUsr()->setServRep(topic);
	chanNameUpdate(&chan);
	if (!chan.getMsgHistory().empty())
		getUsr()->setVectorRep(chan.getMsgHistory());

}

void  Server::joinChan(std::string request){

	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		if (request == (*it).getName()){
			if (!isDuplicatePtr((*it).getUsers(), getUsr()) \
					&& (!isDuplicatePtr((*it).getOperators(), getUsr()))){
				(*it).addUsr(getUsr());
				joinMsg(*it);
			}
			return;
		}
	}
	Channel chan(request, this, getUsr());
	_channels.push_back(chan);
	joinMsg(chan);
}

void	Server::chanTopic(std::string request){
	std::string dest_chan = request.substr(0, request.find(' ') + 1);
	std::string topic = request.substr(request.find(":") + 1);
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		if (dest_chan == (*it).getName()){
			if (!isChanMember(*it))
				return;
			std::string str = ":" + getUsr()->getUsername() + \
				"@127.0.0.1 TOPIC " + dest_chan + ": Channel topic was changed by " + \
					getUsr()->getNickname() + ". New topic is : \"" + topic + "\"\r\n";
			setChanRPL(str, *it);
			(*it).setTopic(topic);
			return;
		}
	}
	std::string err = ":ft_irc.com 401 " + getUsr()->getNickname() \
		+ " " + dest_chan + " : No such channel.\r\n";
	getUsr()->setServRep(err);
	if (!isDuplicatePtr(_waitUsers, getUsr()))
		_waitUsers.push_back(getUsr());
}

void	Server::chanMode(std::string request){
}

void Server::chanMsg(std::string request){
	std::string dest_chan = request.substr(0, request.find(' ') + 1);
	std::string msg = request.substr(request.find(":") + 1);
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		if (dest_chan == (*it).getName()){
			if (!isChanMember(*it))
				return;
			std::string str = ":" +  getUsr()->getUsername() + "@127.0.0.1 PRIVMSG " + dest_chan + " :" + msg + "\r\n";
			(*it).addMsgHistory(str);
			setChanMsgRPL(str, *it);
			return;
		}
	}
	std::string err = ":ft_irc.com 401 " + getUsr()->getNickname() \
		+ " " + dest_chan + " : No such channel.\r\n";
	getUsr()->setServRep(err);
	if (!isDuplicatePtr(_waitUsers, getUsr()))
		_waitUsers.push_back(getUsr());
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
			if (!isChanMember(*it))
				return;
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
