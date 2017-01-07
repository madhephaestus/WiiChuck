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

#include <Arduino.h>
#include "HTTPRequestParser.h"

HTTPRequestParser::HTTPRequestParser () {
	url[0] = '\0';
}

void HTTPRequestParser::parse (char *request) {
	char *p, *q;

#ifdef VERBOSE_REQUEST_PARSER
	DPRINT (F("Parsing request: \""));
	DPRINT (request);
	DPRINTLN (F("\""));
#endif

	url[0] = '\0';
	if ((p = strstr_P (request, PSTR ("GET ")))) {
		if ((q = strchr (p + 4,  ' ')))
			strlcpy (url, p + 4, q - p - 4 + 1 < MAX_URL_LEN ? q - p - 4 + 1 : MAX_URL_LEN);
		else
			strlcpy (url, p + 4, MAX_URL_LEN);

#ifdef VERBOSE_REQUEST_PARSER
		DPRINT (F("Extracted URL: \""));
		DPRINT (url);
		DPRINTLN (F("\""));
#endif
	} else {
		DPRINTLN (F("Cannot extract URL"));
	}
}

char *HTTPRequestParser::get_basename () {
	buffer[0] = '\0';
	char *qMark = strchr (url, '?');
	if (qMark)
		strlcpy (buffer, url, qMark - url + 1 < BUF_LEN ? qMark - url + 1 : BUF_LEN);
	else
		strlcpy (buffer, url, BUF_LEN);

#ifdef VERBOSE_REQUEST_PARSER
	DPRINT (F("Extracted basename: \""));
	DPRINT (buffer);
	DPRINTLN (F("\""));
#endif

	return buffer;
}

char *HTTPRequestParser::get_parameter (const char param[]) {
	char *start;
	boolean found = false;

#ifdef VERBOSE_REQUEST_PARSER
	/* Print this now, because if we got called by
	 * get_parameter(const __FlashStringHelper *), param is actually stored in
	 * buffer and will be overwritten.
	 */
	DPRINT (F("Extracting GET parameter: \""));
	DPRINT (param);
	DPRINT (F("\": \""));
#endif

	for (start = strchr (url, '?'); !found && start; start = strchr (start + 1, '&')) {
		char *end = strchr (start, '=');
		if (end && (end - start - 1) == (int) strlen (param) && strncmp (start + 1, param, end - start - 1) == 0) {
			// Found!
			char *x = strchr (end + 1, '&');
			if (x)
				strlcpy (buffer, end + 1, x - end < BUF_LEN ? x - end : BUF_LEN);
			else
				strlcpy (buffer, end + 1, BUF_LEN);
			found = true;
		}
	}

	if (!found)
		buffer[0] = '\0';

#ifdef VERBOSE_REQUEST_PARSER
	DPRINT (buffer);
	DPRINTLN (F("\""));
#endif

	return buffer;
}

#ifdef ENABLE_FLASH_STRINGS
char *HTTPRequestParser::get_parameter (FlashString param) {
	strncpy_P (buffer, F_TO_PSTR (param), BUF_LEN);
	return get_parameter (buffer);
}
#endif
