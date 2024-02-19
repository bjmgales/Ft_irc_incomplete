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
public:

	Channel(std::string name, Server *serv, Client *user);
    Channel(const Channel &other);
    Channel &operator=(const Channel &other);
    ~Channel();

    std::string getName();

    void setTopic(std::string topic);
    std::string getTopic();

	void addOperator(Client *user);
    std::list<Client *> getOperators();

    void addUsr(Client *user);
    std::list<Client *> getUsers();

    void addMsgHistory(std::string msg);
    std::vector<std::string> getMsgHistory();

    void removeUser(Client *elem);

private:
    Server *_serv;

	std::string _name;
    std::string _topic;
    std::string _password;
    char		_mode;

	std::list<Client *> _users;
    std::list<Client *> _operators;

    std::list<Client *> _invited;
    std::list<Client *> _banned;

    std::vector<std::string> _msgHistory;
};


