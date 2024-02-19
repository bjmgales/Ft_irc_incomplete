#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <iterator>
#include <list>
#include <sstream>
#include "Utils.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "RPL.hpp"


#include <string>


class Server{

private:
	int _port;
	int  _sockfd;
	int	_currUsr;
	int _maxfd;
	std::string _password;
	fd_set					_readfds;
	fd_set					_writefds;

	struct sockaddr_in _info;
	socklen_t _addrSize;
	std::list<Client> _users;
	std::list<Client *> _waitUsers;
	std::list<Channel> _channels;

public:
	//Constructors-Destructor

    Server(int port, std::string password);
    Server(const Server &other);
    Server& operator=(const Server& other);
    ~Server();

    // SET/GET Methods
    int				getSock();
    Client			*getUsr();

	void			setMaxFd(int maxfd);
    int				getMaxFd();

	fd_set			*getReadfds();
    fd_set			*getWritefds();

    socklen_t 		*getAddrSize();
    struct sockaddr *getInfo();

/************ Server Functions *********/

	//server routine
	int init_server(int _port);
    int selectMonitoring();
    void sockReady();

	//socket handling
    void acceptNewClient();
    void clientDisconnect(int i, std::string reason);

	//user handling
	void newUser(std::vector<std::string> command, std::vector<std::string> request);
    void isAuthentified(std::vector<std::string> command, std::vector<std::string> request);

	//receiving request
    std::string getCmd(std::string *request);
    void treatCmd(std::string command, std::string request);
    void handleRequest(std::string buffer);

	//user related commands
	void addUsr(std::string request);
	void addNick(std::string request);

	//channel related commands
	void joinMsg(Channel chan);
	void joinChan(std::string request);
	void chanTopic(std::string request);
	void chanMode(std::string request);
	void chanMsg(std::string request);
	void kickUsr(std::string request);
	void inviteUsr(std::string request);

	//send() replies
	void setRPL(std::string RPL, std::string str);
	void sendRPL();
	void setChanRPL(std::string str, Channel chan);
	void setChanMsgRPL(std::string str, Channel chan);
	void chanNameUpdate(Channel *chan);

	//utils
	bool isChanMember(Channel chan);

	//send msg to user or to channel
	void privMsg(std::string request);

};


#endif
