/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:12:42 by bgales            #+#    #+#             */
/*   Updated: 2024/02/19 10:33:53 by bgales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <iostream>

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
