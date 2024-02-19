
#include "Client.hpp"

Client::Client(){setAddrSize(sizeof(_info));};

Client::Client(const Client &other):_sockFD(other._sockFD),
	_info(other._info), _addrSize(other._addrSize){};

Client & Client::operator =(const Client &other){
	if (this != &other){
		_addrSize = other._addrSize;
		_sockFD = other._sockFD;
		_info = other._info;
	}
	return (*this);
};

Client::~Client(){};

/***********************************************************/

/************************Functions*************************/

void Client::setSock(int fd){
	int flags;
	_sockFD = fd;
	flags = fcntl(_sockFD, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(_sockFD, F_SETFL, flags);
};

void Client::setAddrSize(unsigned int size){
	_addrSize = size;
};

socklen_t *Client::getAddrSize(){
	return &_addrSize;
};

bool Client::isPass(){
	if (_isPass)
		return true;
	return false;
};
void Client::setPass(bool pass){_isPass = pass;}

std::string Client::getUsername(){
	if (_username.empty())
		return ("");
	return _username;
};
void	Client::setUsername(std::string name){_username = name;};
std::string Client::getNickname(){
	if (_nickname.empty())
		return ("");
	return _nickname;
};
void	Client::setNickname(std::string name){_nickname = name;};

int Client::getSock(){
	return _sockFD;
};

struct sockaddr *Client::getInfo(){
	return (struct sockaddr *)&_info;
};
void Client::setSigned(bool i){
	_signedUser = i;
}
bool Client::getSigned(){
	return _signedUser;
}
void Client::setServRep(std::string rep){
	_servReply.push_back(rep);
};
void Client::setVectorRep(std::vector<std::string> rep){
	_servReply.insert(_servReply.end(), rep.begin(), rep.end());
}
std::vector<std::string> Client::getServRep(){return _servReply;};
void Client::clearServRep(){
	_servReply.clear();
}
