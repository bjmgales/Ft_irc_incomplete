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

	/***************Constructors/Destructor********************/

	Server(int port, std::string password):_port(port), _password(password){
		_sockfd = init_server(_port);
	};

	Server(const Server &other):_sockfd(other._sockfd), _info(other._info){};

	Server& operator =(const Server& other){
		if (this != &other){
			_sockfd = other._sockfd;
			_info = other._info;
		}
		return (*this);
	};

	~Server(){};
	/**********************************************************/

	/*************************SET/GET***************************/

	void setMaxFd(int maxfd){
		_maxfd = maxfd;
	};

	socklen_t *getAddrSize(){
		return &_addrSize;
	};

	int getSock(){
		return _sockfd;
	};

	int getMaxFd(){
		return _maxfd;
	}

	Client *getUsr(){
		for (std::list<Client>::iterator it = _users.begin();\
				it != _users.end(); it++){
				if (_currUsr == (*it).getSock())
					return &(*it);
		}
		return NULL;
	}

	fd_set *getReadfds(){
		return &_readfds;
	};

	fd_set *getWritefds(){
		return &_writefds;
	};
	struct sockaddr *getInfo(){
		return (struct sockaddr *)&_info;
	};
	/**********************************************************/

	/************************Functions*************************/



	int init_server(int _port){
		int enable = 1;
		_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (_sockfd < 0){
			std::cout << "Error on serv socket" << std::endl;
			exit (0);
		}
		FD_SET(_sockfd, &_readfds);
		_info.sin_family = AF_INET;
		_info.sin_port = htons((_port));
		_info.sin_addr.s_addr = htonl(INADDR_ANY);
		setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		if (bind(_sockfd, (struct sockaddr *)&_info, sizeof(_info)) < 0){
			std::cerr << "Error: bind() failure." << std::endl;
			return (-1);
		}

		if (listen(_sockfd, 1000) < 0){
			std::cerr << "Error: listen() failure." << std::endl;
			return (-1);
		}
		_maxfd = getSock();

		return (_sockfd);
	}

	void acceptNewClient(){
		Client newClient;

		newClient.setSock(accept( getSock(), newClient.getInfo(), newClient.getAddrSize()));
		if (newClient.getSock() < 0){
			std::cout << "Error : Accept error on client socket." << std::endl;
			return;
		}
		else{
			newClient.setSigned(false);
			_users.push_back(Client(newClient));
			FD_SET(newClient.getSock(), &_readfds);
			std::cout << "Connection succeded on socket number " << newClient.getSock() << std::endl;
		}

		for (std::list<Client>::iterator it = _users.begin(); it != _users.end();){
			if ((*it).getSock() > _maxfd)
				_maxfd = (*it).getSock();
			it++;
		}
	}

	void clientDisconnect(int i, std::string reason){
		for (std::list<Client>::iterator it = _users.begin(); it != _users.end();){
			if ((*it).getSock() == i)
				(*it).setSock(0);
			it++;
		}
		FD_CLR(i, &_readfds);
		close (i);
		std::cout << reason + " " << i << std::endl;
	}

	int selectMonitoring(){
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

	std::string getCmd(std::string *request){
		std::string command;
		if (request->find(' ') != std::string::npos){
			command = request->substr(0, request->find(' '));
			*request = request->substr(request->find(' ') + 1);
		}

		cleanCarriageRet(request);
		return (command);
	}

	void newUser(std::vector<std::string> command, std::vector<std::string> request){
		for (int i = 0; i < command.size(); i++){
			if (command[i] == "USER")
				addUsr(request[i]);
			else if (command[i] == "NICK")
				addNick(request[i]);
		std::cout << getUsr()->getNickname() << std::endl;;
		}
		if (!getUsr()->getNickname().empty() && !getUsr()->getUsername().empty()){
			getUsr()->setSigned(true);
			setRPL(RPL_WELCOME, ("Welcome to my irc server " + getUsr()->getNickname()));
			setRPL(RPL_YOURHOST, ("Your host is ft_irc_42, running version 0.42"));
			setRPL(RPL_CREATED, "This server was created moments ago");
			setRPL(RPL_MYINFO, "ft_irc v:0.42 io iobl");
			setRPL(RPL_MOTDSTART, "ft_irc Bonjour à tous et bienvenue !");
			setRPL(RPL_MOTD, "ft_irc Le diction du jour : \"un tiens vaut mieux que deux tu l'auras\"");
			setRPL(RPL_ENDOFMOTD, "End of welcome message");
		}
		else{
			const char err[] = " ERROR : need both username and nickname\r\n";
			send(_currUsr, err, strlen(err), 0);
		}
	}
	void treatCmd(std::string command, std::string request){
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

	void isAuthentified(std::vector<std::string> command, std::vector<std::string> request){

		if (getUsr()->isPass() == false){
			for (int i = 0; i < request.size(); i++){
				if (command[i] == "PASS" && request[i] == _password){
					getUsr()->setPass(true);
					isAuthentified(command, request);
					return;
				}

			}
			send(_currUsr, "ERROR : awaiting server password\r\n", strlen("ERROR : awaiting server password\r\n"), 0);
		}
		else{
			if (getUsr()->getSigned() == false)
				newUser(command, request);
			else
				for (int i = 0; i < command.size(); i++)
					treatCmd(command[i], request[i]);
		}

	}
	void handleRequest(std::string buffer){
		std::vector<std::string> request;
		std::vector<std::string> command;
		std::stringstream ss(buffer);
		std::string tmp;

		cleanCarriageRet(&buffer);
		std::cout << "Received: " << buffer << std::endl;
		if (buffer.empty()){
			std::cout << "Expecting : PASS NICK USER"<< std::endl;
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

	void sockReady(){
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

	/**********************************************************/
	void addUsr(std::string request);
	void addNick(std::string request);
	void joinChan(std::string request);
	void privMsg(std::string request);
	void kickUsr(std::string request);
	void inviteUsr(std::string request);
	void chanTopic(std::string request);
	void chanMode(std::string request);
	void sendRPL();
	void setRPL(std::string RPL, std::string str);
	void joinMsg(Channel chan);
	void setChanRPL(std::string str, Channel chan);
	void chanNameUpdate(Channel *chan);
	void chanMsg(std::string request);
};


#endif
