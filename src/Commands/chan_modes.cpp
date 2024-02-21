#include "Server.hpp"

void	Server::inviteOnlyChan(Channel chan, std::string mode){
	std::string str =  + ":" + getUsr()->getNickname()\
		+ "@127.0.0.1 MODE " + chan.getName() + mode + "\r\n";
	setChanRPL(str, chan);
	if (mode == "+i")
		chan.setInviteOnly(true);
	else
		chan.setInviteOnly(false);
}

void	Server::topicOperators(Channel chan, std::string mode){
	std::string str =  + ":" + getUsr()->getNickname()\
		+ "@127.0.0.1 MODE " + chan.getName() + mode + "\r\n";
	setChanRPL(str, chan);
	if (mode == "+t")
		chan.setOpTopic(true);
	else
		chan.setOpTopic(false);

}

void	Server::passwordChan(Channel chan, std::string mode){
	std::string str =  + ":" + getUsr()->getNickname()\
		+ "@127.0.0.1 MODE " + chan.getName() + mode + "\r\n";
	setChanRPL(str, chan);
	if (mode == "+p"){
		chan.setPassword("test");
		chan.setBoolPassword(true);
	}
	else
		chan.setBoolPassword(false);
}

void	Server::ChanOperators(Channel chan, std::string mode){

}

void	Server::chanMode(std::string request){
	std::string dest_chan = request.substr(0, request.find(' ') + 1);
	std::string mode = request.substr(request.find(' ') + 1);

	if ( mode.empty() || (mode.length() != 2 && !isSet(mode[1], "po")) || !isSet(mode[0], "+-") || !isSet(mode[1], "itkol")){
		std::string err =  ":ft_irc.com 401 " + getUsr()->getNickname() \
		+ " " + dest_chan + " : mode usage: /mode [-+][itkol]\r\n";
		getUsr()->setServRep(err);
		if (!isDuplicatePtr(_waitUsers, getUsr()))
			_waitUsers.push_back(getUsr());
		return;
	}

	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		if (dest_chan == (*it).getName()){
			if (!isChanMember(*it) || !isChanOperator(*it))
				return;
			switch (mode[1])
			{
				case 'i':
					inviteOnlyChan(*it, mode);
					break;
				case 't':
					topicOperators(*it, mode);
				case 'k':
					passwordChan(*it, mode);
				case 'o':
					ChanOperators(*it, mode);
			}
		}
	}
}
