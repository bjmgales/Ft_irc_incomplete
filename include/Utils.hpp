/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgales <bgales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:14:16 by bgales            #+#    #+#             */
/*   Updated: 2024/02/18 15:28:06 by bgales           ###   ########.fr       */
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
