/***************************************************************************
 *   This file is part of Webbino                                          *
 *                                                                         *
 *   Copyright (C) 2012-2016 by SukkoPera                                  *
 *                                                                         *
 *   Webbino is free software: you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Webbino is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Webbino. If not, see <http://www.gnu.org/licenses/>.       *
 ***************************************************************************/

#ifndef _INTERFACE_H_INCLUDED
#define _INTERFACE_H_INCLUDED

#include <Arduino.h>
#include <IPAddress.h>
#include <WebbinoCore/WebClient.h>


class NetworkInterface {
public:
	virtual WebClient* processPacket () = 0;

	virtual boolean usingDHCP () = 0;

	virtual byte* getMAC () = 0;

	virtual IPAddress getIP () = 0;

	virtual IPAddress getNetmask () = 0;

	virtual IPAddress getGateway () = 0;
};

#endif
