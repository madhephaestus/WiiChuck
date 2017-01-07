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

#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include <webbino_config.h>
#include "NetworkInterface.h"
#include "HTTPRequestParser.h"

// http://arduiniana.org/libraries/pstring/
#include <PString.h>

#if defined (WEBBINO_ENABLE_SD)
#include <SD.h>
#elif defined (WEBBINO_ENABLE_SDFAT)
#include <SdFat.h>
#endif


class WebClient;


/******************************************************************************/

typedef void (*PageFunction) (HTTPRequestParser& request);

struct Page {
	PGM_P name;
	PGM_P content;
	PageFunction function;

	// Methods that (try to) hide the complexity of accessing PROGMEM data
	PGM_P getName () const {
		return reinterpret_cast<PGM_P> (pgm_read_ptr (&(this -> name)));
	}

	PageFunction getFunction () const {
		return reinterpret_cast<PageFunction> (pgm_read_ptr (&(this -> function)));
	}

	PGM_P getContent () const {
		return reinterpret_cast<PGM_P> (pgm_read_ptr (&(this -> content)));
	}
};

/******************************************************************************/


class PageContent {
public:
	virtual char getNextByte () = 0;
};

/******************************************************************************/


class FlashContent: public PageContent {
private:
	const Page& page;
	PGM_P next;

public:
	FlashContent (const Page* p);

	char getNextByte () override;
};

/******************************************************************************/


#if defined (WEBBINO_ENABLE_SD) || defined (WEBBINO_ENABLE_SDFAT)

struct SDContent: public PageContent {
private:
	File file;

public:
	SDContent (const char* filename);
	~SDContent ();

	char getNextByte () override;
};

#endif

/******************************************************************************/


#ifdef ENABLE_TAGS

typedef PString& (*TagEvaluateFn) (void* data);

struct ReplacementTag {
	PGM_P name;				// Max length: MAX_TAG_LEN
	TagEvaluateFn function;
	void *data;

	// Methods that (try to) hide the complexity of accessing PROGMEM data
	PGM_P getName () const {
		return reinterpret_cast<PGM_P> (pgm_read_ptr (&(this -> name)));
	}

	TagEvaluateFn getFunction () const {
		return reinterpret_cast<TagEvaluateFn> (pgm_read_ptr (&(this -> function)));
	}

	void *getData () const {
		return reinterpret_cast<void *> (const_cast<void *> (pgm_read_ptr (&(this -> data))));
	}
};

/* Macros that simplify things
 * Note that max length of tag name is MAX_TAG_LEN (24)
 */
#define REPTAG_STR(tag) #tag
#define REPTAG_STR_VAR(tag) _rtStr_ ## tag
//#define REPTAG_VAR(tag) _rtTag_ ## tag
//#define REPTAG_PTR(tag) &REPTAG_VAR(tag)

#define RepTagWithArg(var, tag, fn, arg) \
                const char REPTAG_STR_VAR(tag)[] PROGMEM = REPTAG_STR(tag); \
                const ReplacementTag var PROGMEM = {REPTAG_STR_VAR(tag), fn, reinterpret_cast<void *> (arg)};

#define RepTagNoArg(var, tag, fn) RepTagWithArg(var, tag, fn, NULL)

// http://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define EasyReplacementTag(...) GET_MACRO (__VA_ARGS__, RepTagWithArg, RepTagNoArg) (__VA_ARGS__)

typedef const ReplacementTag* const EasyReplacementTagArray;

#endif

/******************************************************************************/


class WebServer {
private:
	NetworkInterface* netint;

	const Page* const *pages;

#ifdef ENABLE_TAGS
	const ReplacementTag* const * substitutions;
#endif

	void sendPage (WebClient* client);

	void sendContent (WebClient* client, PageContent* content);

	const Page *getPage (const char* name) const;

#ifdef ENABLE_TAGS
	PString* findSubstitutionTag (const char* tag) const;

	char *findSubstitutionTagGetParameter (HTTPRequestParser& request, const char* tag);
#endif

public:
	// Return value here is only meaningful if using SD
	boolean begin (NetworkInterface& _netint, const Page* const _pages[] = NULL
#ifdef ENABLE_TAGS
		, const ReplacementTag* const _substitutions[] = NULL
#endif
#if defined (WEBBINO_ENABLE_SD) || defined (WEBBINO_ENABLE_SDFAT)
		, int8_t pin = -1
#endif
	);

	boolean loop ();
};

#endif
