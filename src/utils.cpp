/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:12:42 by bgales            #+#    #+#             */
/*   Updated: 2024/02/19 22:15:50 by bgales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <iostream>

bool isAlnum(std::string str){
	for(ssize_t i = 0; i < str.length(); ++i){
		if (!isalnum(str[i])) {
			std::cerr << "Error: usage: password must be alpha-numeric."<< std::endl;
			return (false);
		}
	}
	return (true);
}

bool isDigit(std::string str){
	for(ssize_t i = 0; i < str.length(); ++i){
		if (!isdigit(str[i])) {
			std::cerr << "Error: usage: port must contain digits only." << std::endl;
			return (false);
		}
	}
	return (true);
}

void cleanCarriageRet(std::string *str){

	if (str->find("\r") != std::string::npos){
		str->erase(str->find("\r"));
	}
	if (str->find("\n") != std::string::npos){
		str->erase(str->find("\n"));
	}
}

bool isDuplicate(std::list<Client> set, Client elem){

	for (std::list<Client>::iterator it = set.begin(); it != set.end(); it++){
		if (it->getUsername() == elem.getUsername())
			return (true);
	}
	return (false);
}

bool isDuplicatePtr(std::list<Client *> set, Client *elem){
	for (std::list<Client *>::iterator it = set.begin(); it != set.end(); it++){
		if ((*it)->getUsername() == elem->getUsername())
			return (true);
	}
	return (false);
}
