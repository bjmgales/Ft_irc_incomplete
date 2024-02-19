#include "Server.hpp"

/***************Constructors/Destructor********************/

Server::Server(int port, std::string password): _port(port), _password(password) {
    _sockfd = init_server(_port);
}

Server::Server(const Server &other): _sockfd(other._sockfd), _info(other._info) {}

Server& Server::operator=(const Server& other) {
    if (this != &other) {
        _sockfd = other._sockfd;
        _info = other._info;
    }
    return *this;
}

Server::~Server() {}
/**********************************************************/

/*************************SET/GET***************************/

int Server::getSock() {
    return _sockfd;
}

Client* Server::getUsr() {
    for (std::list<Client>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (_currUsr == (*it).getSock())
            return &(*it);
    }
    return NULL;
}


void Server::setMaxFd(int maxfd) {
    _maxfd = maxfd;
}

int Server::getMaxFd() {
    return _maxfd;
}


fd_set* Server::getReadfds() {
    return &_readfds;
}

fd_set* Server::getWritefds() {
    return &_writefds;
}

socklen_t* Server::getAddrSize() {
    return &_addrSize;
}

struct sockaddr* Server::getInfo() {
    return (struct sockaddr*)&_info;
}
/**********************************************************/

/************************Functions*************************/

int Server::init_server(int _port) {
    int enable = 1;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0) {
        std::cout << "Error on serv socket" << std::endl;
        exit(0);
    }
    FD_SET(_sockfd, &_readfds);
    _info.sin_family = AF_INET;
    _info.sin_port = htons(_port);
    _info.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (bind(_sockfd, (struct sockaddr*)&_info, sizeof(_info)) < 0) {
        std::cerr << "Error: bind() failure." << std::endl;
        return -1;
    }

    if (listen(_sockfd, 1000) < 0) {
        std::cerr << "Error: listen() failure." << std::endl;
        return -1;
    }
    _maxfd = getSock();

    return _sockfd;
}

int Server::selectMonitoring(){
	struct timeval t;

	t.tv_sec = 1;
	t.tv_usec = 0;
	_maxfd = _sockfd;
	for (std::list<Client>::iterator it = _users.begin(); it != _users.end();){
		if ((*it).getSock() > 3){
			FD_SET((*it).getSock(), &_readfds);
		}
		if (_maxfd < (*it).getSock())
			_maxfd = (*it).getSock();
		it++;
	}
	for (std::list<Client *>::iterator it = _waitUsers.begin(); it != _waitUsers.end();){
		if ((*it)->getSock() > 3){
			FD_SET((*it)->getSock(), &_writefds);
		}
		it++;
	}
	FD_SET(_sockfd, &_readfds);
	return select(getMaxFd() + 1, &_readfds, &_writefds,  NULL, NULL);
}

void Server::sockReady(){
	for (int i = 0; i <= _maxfd; ++i){
		if (FD_ISSET(i, &_writefds)){
			_currUsr = i;
			sendRPL();

		}
		else if (FD_ISSET(i, &_readfds)){
			if (i == getSock()){
				acceptNewClient();
				return;
			}
			else
			{
				_currUsr = i;
				char buffer[1024];
				ssize_t size = recv(i, buffer, 1024, 0);
				if (size < 0){
					std::cout << "No req" << std::endl;
					return;
				}
				else if (size == 0){
					clientDisconnect(i, "Client disconnected from socket");

				}
				else{
					buffer[size] = 0;
					handleRequest(std::string(buffer));

				}
			}
		}
	}

}




