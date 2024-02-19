#include "Channel.hpp"

/*******************CONSTRUCTORS/DESTRUCTOR*****************************/

Channel::Channel(std::string name, Server *serv, Client *user): _name(name), _serv(serv) {
    _operators.push_back(user);
}

Channel::Channel(const Channel & other) {
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

Channel& Channel::operator=(const Channel & other) {
    if (this != &other) {
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
    return *this;
}

Channel::~Channel() {}

void Channel::removeUser(Client *elem) {
    for (std::list<Client *>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (*it == elem) {
            _users.erase(it);
            return;
        }
    }
}
/***************************************************************************/

/*******************************SET/GET*************************************/

std::string Channel::getName() {
    return _name;
}


void Channel::setTopic(std::string topic) {
    _topic = topic;
}

void Channel::addUsr(Client *user) {
    _users.push_back(user);
}

void Channel::addOperator(Client *user){
	_operators.push_back(user);
}

std::string Channel::getTopic() {
    if (_topic.empty())
        _topic = "undefined";
    return _topic;
}

std::list<Client *> Channel::getOperators() {
    return _operators;
}

std::list<Client *> Channel::getUsers() {
    return _users;
}

void Channel::addMsgHistory(std::string msg) {
    _msgHistory.push_back(msg);
}

std::vector<std::string> Channel::getMsgHistory() {
    return _msgHistory;
}
/***************************************************************************/
