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

#include "WebServer.h"
#include "WebClient.h"
#include <webbino_debug.h>

#if defined (WEBBINO_ENABLE_SDFAT)
static SdFat SD;
#endif


/******************************************************************************/

FlashContent::FlashContent (const Page* p): page (*p), next (p -> getContent ()) {
}

char FlashContent::getNextByte () {
	return pgm_read_byte (next++);
}

/******************************************************************************/

#if defined (WEBBINO_ENABLE_SD) || defined (WEBBINO_ENABLE_SDFAT)

SDContent::SDContent (const char* filename) {
	file = SD.open (filename);
}

SDContent::~SDContent () {
	file.close ();
}

char SDContent::getNextByte () {
	if (file.available ()) {
		return file.read ();
	} else {
		return '\0';
	}
}

#endif

/******************************************************************************/


#define HEADER_START "HTTP/1.0 "
#define REDIRECT_HEADER "301 Moved Permanently\r\nLocation: "
#define OK_HEADER "200 OK\r\nContent-Type: text/html"		// \r\nPragma: no-cache"
#define NOT_FOUND_HEADER "404 Not Found\r\nContent-Type: text/html"
#define HEADER_END "\r\n\r\n"


boolean WebServer::begin (NetworkInterface& _netint, const Page* const _pages[]
#ifdef ENABLE_TAGS
		, const ReplacementTag* const _substitutions[]
#endif
#if defined (WEBBINO_ENABLE_SD) || defined (WEBBINO_ENABLE_SDFAT)
		, int8_t pin
#endif
		) {

	boolean ret = true;

	netint = &_netint;

	pages = _pages;

#ifndef WEBBINO_NDEBUG
	DPRINTLN (F("Pages available in flash memory:"));
	const Page *p = NULL;
	for (byte i = 0; pages && (p = reinterpret_cast<const Page *> (pgm_read_ptr (&pages[i]))); i++) {
		DPRINT (i);
		DPRINT (F(". "));
		DPRINTLN (PSTR_TO_F (p -> getName ()));
	}
#endif

#ifdef ENABLE_TAGS
	substitutions = _substitutions;

#ifndef WEBBINO_NDEBUG
	DPRINTLN (F("Tags available:"));
	const ReplacementTag* sub;
	for (byte i = 0; substitutions && (sub = reinterpret_cast<const ReplacementTag *> (pgm_read_ptr (&substitutions[i]))); i++) {
		DPRINT (i);
		DPRINT (F(". "));
		DPRINTLN (PSTR_TO_F (sub -> getName ()));
	}
#endif

#endif

#if defined (WEBBINO_ENABLE_SD) || defined (WEBBINO_ENABLE_SDFAT)
	if (pin >= 0) {
		DPRINT (F("Initializing SD card..."));
		if (!SD.begin (pin)) {
			DPRINTLN (F(" failed"));
			ret = false;
		}
		DPRINTLN (F(" done"));
	}
#endif

	return ret;
}

const Page* WebServer::getPage (const char* name) const {
	const Page *p = NULL;

	// For some reason, if we make i a byte here, the code uses 8 more bytes, so don't!
	for (unsigned int i = 0; pages && (p = reinterpret_cast<const Page *> (pgm_read_ptr (&pages[i]))); i++) {
		if (strcmp_P (name, p -> getName ()) == 0)
			break;
	}

	return p;
}

void WebServer::sendPage (WebClient* client) {
	client -> initReply ();

	unsigned int l = strlen (client -> request.url);
	if (l == 0 || client -> request.url[l - 1] == '/') {
		// Request for "/", redirect
		DPRINT (F("Redirecting to "));
		DPRINT (client -> request.url);
		DPRINTLN (F(REDIRECT_ROOT_PAGE));

		client -> print (F(HEADER_START REDIRECT_HEADER));
		if (l == 0)
			client -> print ('/');
		else
			client -> print (client -> request.url);
		client -> print (F(REDIRECT_ROOT_PAGE HEADER_END));
	} else {
		const Page *page;
		char *pagename = client -> request.get_basename ();

#if defined (WEBBINO_ENABLE_SD) || defined (WEBBINO_ENABLE_SDFAT)
		if (SD.exists (pagename)) {
			DPRINT (F("Sending page from SD file "));
			DPRINTLN (pagename);

			SDContent content = SDContent (pagename);
			sendContent (client, &content);
		} else
#endif
		if ((page = getPage (pagename))) {
			// Call page function
			PageFunction func = page -> getFunction ();
			if (func)
				func (client -> request);

			FlashContent content = FlashContent (page);
			sendContent (client, &content);
		} else {
			client -> print (F(HEADER_START NOT_FOUND_HEADER HEADER_END));

			client -> print (F("<html><body><h3>No such page: \""));
			client -> print (pagename);
			client -> print (F("\"</h3></body></html>"));
		}
	}

	client -> sendReply ();
}

#ifdef ENABLE_TAGS
PString* WebServer::findSubstitutionTag (const char *tag) const {
	const ReplacementTag *sub;
	PString* ret = NULL;

	for (byte i = 0; !ret && (sub = reinterpret_cast<const ReplacementTag *> (pgm_read_ptr (&substitutions[i]))); i++) {
		if (strcmp_P (tag, sub -> getName ()) == 0) {
			PString& pb = (sub -> getFunction ()) (sub -> getData ());
			ret = &pb;
		}
	}

	return ret;
}

char *WebServer::findSubstitutionTagGetParameter (HTTPRequestParser& request, const char *tag) {
	return request.get_parameter (tag);
}
#endif

// Read the page, perform tag substitutions and send it over
// FIXME: Handle unterminated tags
void WebServer::sendContent (WebClient* client, PageContent* content) {
	// Send headers
	client -> print (F(HEADER_START OK_HEADER HEADER_END));

	char c;
#ifdef ENABLE_TAGS
	char tag[MAX_TAG_LEN];
	int8_t tagLen = -1;			// If >= 0 we are inside a tag
#endif
	while ((c = content -> getNextByte ())) {
#ifdef ENABLE_TAGS
		if (tagLen >= 0) {
			if (c == TAG_CHAR) {
				DPRINT (F("Processing replacement tag: \""));
				DPRINT (tag);
				DPRINTLN (F("\""));

				boolean found = false;
				if (strncmp_P (tag, PSTR ("GETP_"), 5) == 0) {
					char* rep = findSubstitutionTagGetParameter (client -> request, tag + 5);
					if (rep) {
						DPRINT (F("Replacement is: \""));
						DPRINT (rep);
						DPRINTLN (F("\""));

						client -> print (rep);
						found = true;
					}
				} else {
					PString* pstr = findSubstitutionTag (tag);
					if (pstr) {
						DPRINT (F("Replacement is: \""));
						DPRINT (*pstr);
						DPRINTLN (F("\""));

						client -> print (*pstr);
						pstr -> begin ();		// Reset for next usage
						found = true;
					}
				}

				if (!found) {
					// Tag not found, emit it
					DPRINTLN (F("Tag not found"));

					client -> print (TAG_CHAR);
					client -> print (tag);
					client -> print (TAG_CHAR);
				}

				// Tag complete
				tagLen = -1;
			} else if (tagLen < MAX_TAG_LEN - 1) {
				tag[tagLen++] = c;
				tag[tagLen] = '\0';
			} else {
				// Tag too long, emit what we got so far
				// FIXME: This will detect a fake tag at the closing TAG_CHAR
				client -> print (TAG_CHAR);
				client -> print (tag);
				client -> print (c);
				tagLen = -1;
			}
		} else {
			if (c == TAG_CHAR) {
				// Check if we have a tag to be replaced
				tag[0] = '\0';
				tagLen = 0;
			} else {
				client -> print (c);
			}
		}
#else
		client -> print (c);
#endif
	}
}

boolean WebServer::loop () {
	WebClient *c = netint -> processPacket ();
	if (c != NULL) {
		// Got a client with a request, process it
		DPRINT (F("Request for \""));
		DPRINT (c -> request.url);
		DPRINTLN (F("\""));

		sendPage (c);
	}

	return c != NULL;
}
