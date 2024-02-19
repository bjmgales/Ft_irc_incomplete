/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:01:12 by bgales            #+#    #+#             */
/*   Updated: 2024/02/16 12:03:12 by bgales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"

class Server;

class Channel
{
private:
	Server				*_serv;
	std::string 		_name;
	std::string 		_topic;
	std::list<Client *> 	_users;
	std::list<Client *> 	_operators;
	char 				_mode;
	std::string			_password;
	std::list<Client *> 	_invited;
	std::list<Client *>	_banned;
	std::vector<std::string> _msgHistory;



public:
	/***************Constructors/Destructor********************/

	Channel(std::string name, Server *serv, Client *user):_name(name), _serv(serv){
		_operators.push_back(user);
	};

	Channel(const Channel & other){
		_name = other._name;
		_topic = other._topic;
		_users = other._users;
		_operators = other._operators;
		_mode = other._mode;
		_password = other._password;
		_invited = other._invited;
		_banned = other._banned;
		_msgHistory = other._msgHistory;
		_serv = other._serv;
	};

	Channel & operator = (const Channel & other){
		if (this != &other){
			_name = other._name;
			_topic = other._topic;
			_users = other._users;
			_operators = other._operators;
			_mode = other._mode;
			_password = other._password;
			_invited = other._invited;
			_banned = other._banned;
			_msgHistory = other._msgHistory;
			_serv = other._serv;
		}
		return (*this);
	};

	~Channel(){};
	/**********************************************************/

	void removeUser(Client *elem){
		for (std::list<Client *>::iterator it = _users.begin(); it != _users.end(); it++){
			if (*it == elem){
				_users.erase(it);
				return;
			}
		}
	}
	/***********************Set/Get****************************/

	std::string getName(){return _name;};
	void	addUsr(Client *user){
		_users.push_back(user);
	}
	void setTopic(std::string topic){_topic = topic;};
	std::string getTopic(){
		if (_topic.empty())
			_topic = "undefined";
		return _topic;
	}
	std::list<Client *> getOperators(){
		return _operators;
	}

	std::list<Client *> getUsers(){
		return _users;
	}
};


