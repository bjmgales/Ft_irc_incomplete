/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:22:35 by bgales            #+#    #+#             */
/*   Updated: 2024/02/19 20:57:54 by bgales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


class Client
{
private:
	int 				 				_sockFD;
	std::string							_nickname;
	std::string							_username;
	bool								_isPass;
	bool								_signedUser;
	socklen_t 							_addrSize;
	struct sockaddr_in 					_info;
	std::vector<std::string>			_servReply;

public:

/***************Constructors/Destructor********************/

Client();
Client(const Client &other);
Client &operator =(const Client &other);
~Client();

/***************Set/Get********************/

void setSock(int fd);
int getSock();

void setAddrSize(unsigned int size);
socklen_t *getAddrSize();
struct sockaddr *getInfo();

void setPass(bool pass);
bool isPass();

void setUsername(std::string name);
std::string getUsername();

void setNickname(std::string name);
std::string getNickname();

void setSigned(bool i);
bool getSigned();

//prepare what will be send() to server
void setServRep(std::string rep);
void setVectorRep(std::vector<std::string> rep);
std::vector<std::string> getServRep();
void clearServRep();
};

