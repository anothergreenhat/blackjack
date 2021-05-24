/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
 * Copyright (C) 2021 Joseph Madera <>
 * 
 * blackjack is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * blackjack is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
using namespace std;

#include "player.h"
#include "dealer.h"

int main()
{
	player me,com; dealer_operations dealer; 
	cout << '\n' << "Hello there!" << '\n';
	cout << "This is a game of Black Jack." << '\n' << '\n';
    dealer.start_of_play(me,com);
	return 0;
}

