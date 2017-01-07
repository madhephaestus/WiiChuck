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

#ifndef _HTTPREQUESTPARSER_H_
#define _HTTPREQUESTPARSER_H_

#include <webbino_config.h>
#include <webbino_debug.h>


class HTTPRequestParser {
private:
	char buffer[BUF_LEN];

public:
	HTTPRequestParser ();

	char url[MAX_URL_LEN];

	void parse (char *request);

	char *get_basename ();

	char *get_parameter (const char param[]);

#ifdef ENABLE_FLASH_STRINGS
	char *get_parameter (FlashString param);
#endif
};

#endif
