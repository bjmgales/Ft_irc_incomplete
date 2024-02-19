/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:22:35 by bgales            #+#    #+#             */
/*   Updated: 2024/02/18 20:28:37 by bgales           ###   ########.fr       */
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

	Client(){setAddrSize(sizeof(_info));};

	Client(const Client &other):_sockFD(other._sockFD),
		_info(other._info), _addrSize(other._addrSize){};

	Client & operator =(const Client &other){
		if (this != &other){
			_addrSize = other._addrSize;
			_sockFD = other._sockFD;
			_info = other._info;
		}
		return (*this);
	};

	~Client(){};

	/***********************************************************/

	/************************Functions*************************/

	void setSock(int fd){
		int flags;
		_sockFD = fd;
		flags = fcntl(_sockFD, F_GETFL, 0);
		flags |= O_NONBLOCK;
		fcntl(_sockFD, F_SETFL, flags);

	};

	void setAddrSize(unsigned int size){
		_addrSize = size;
	};

	socklen_t *getAddrSize(){
		return &_addrSize;
	};

	bool isPass(){
		if (_isPass)
			return true;
		return false;
	};
	void setPass(bool pass){_isPass = pass;}

	std::string getUsername(){
		if (_username.empty())
			return ("");
		return _username;
		};
	void		setUsername(std::string name){_username = name;};
	std::string getNickname(){
		if (_nickname.empty())
			return ("");
		return _nickname;
	};
	void		setNickname(std::string name){_nickname = name;};

	int getSock(){
		return _sockFD;
	};

	struct sockaddr *getInfo(){
		return (struct sockaddr *)&_info;
	};
	void setSigned(bool i){
		_signedUser = i;
	}
	bool getSigned(){
		return _signedUser;
	}
	void setServRep(std::string rep){
		_servReply.push_back(rep);
	};
	std::vector<std::string> getServRep(){return _servReply;};
	void clearServRep(){
		_servReply.clear();
	}
	/***********************************************************/

};

