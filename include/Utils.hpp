/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:14:16 by bgales            #+#    #+#             */
/*   Updated: 2024/02/21 11:33:08 by bgales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdio.h>
#include <string>
#include <list>
#include "Client.hpp"

bool isDuplicatePtr(std::list<Client *> set, Client *elem);
bool isDuplicate(std::list<Client> set, Client elem);
void cleanCarriageRet(std::string *str);
bool isAlnum(std::string str);
bool isDigit(std::string str);
bool isSet(char c, std::string set);
