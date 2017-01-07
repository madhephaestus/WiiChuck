#!/usr/bin/env python

# Run this script like this:
# ./html2h.py webroot > html.h
#
# Note that this is untested on Windows/OSX!

import os
import sys
import argparse

parser = argparse.ArgumentParser (description = 'Convert a website to be put into flash memory for use with Webbino')
parser.add_argument ('webroot', metavar = "WEBROOT", help = "Path to website root directory")
parser.add_argument ('--nostrip', "-n", action = 'store_true', default = False,
                     help = "Do not strip CR/LF/TABs")

args = parser.parse_args ()

# The above will raise an error if webroot was not specified, so we can
# assume it was
os.chdir (args.webroot)

# https://www.safaribooksonline.com/library/view/python-cookbook/0596001673/ch04s16.html
def splitall(path):
    allparts = []
    while 1:
        parts = os.path.split(path)
        if parts[0] == path:  # sentinel for absolute paths
            allparts.insert(0, parts[0])
            break
        elif parts[1] == path: # sentinel for relative paths
            allparts.insert(0, parts[1])
            break
        else:
            path = parts[0]
            allparts.insert(0, parts[1])
    return allparts

def process_file (filename):
	print >> sys.stderr, "Processing file: %s" % filename

	try:
		with open (filename, 'r') as fp:
			# Make up a unique ID for every file to use in C identifiers
			parts = splitall (filename[2:])
			parts = [parts[0]] + [x.capitalize () for x in parts[1:]]
			code = "".join (parts)
			code = code.replace ('.', '_')

			print "const char %s_name[] PROGMEM = \"%s\";" % (code, filename[1:])
			print
			print "const char %s[] PROGMEM = {" % code
			i = 0
			b = fp.read (1)
			while b:
				if args.nostrip or (b != '\n' and b != '\r' and b != '\t'):
					if i % 8 == 0:
						print "\t",
					print "0x%02x, " % ord (b),
					i = i + 1
					if i % 8 == 0:
						print ""
				b = fp.read (1)

			print "0x00"
			print "};"
			print
			print "// unsigned int %s_len = %u;" % (code, i + 1)
			print
	except IOError as ex:
		print "Cannot open file %s: %s" % (filename, str (ex))
		code = None

	return code

def process_dir (dirpath):
	print >> sys.stderr, "Processing directory: %s" % dirpath
	idents = []
	for filename in sorted (os.listdir (dirpath)):
		fullfile = os.path.join (dirpath, filename)
		if os.path.isfile (fullfile):
			ident = process_file (fullfile)
			if ident is not None:
				idents.append (ident)
		elif os.path.isdir (fullfile):
			idents.extends (process_dir (fullfile))
		else:
			print "Skipping %s" % filename
	return idents


### MAIN ###

idents = process_dir (".")
n_pages = len (idents)

print >> sys.stderr, "Total files processed: %d" % n_pages

if n_pages > 0:
	# Help with code to be put in sketch
	print >> sys.stderr
	print >> sys.stderr, "Put the following in your sketch:"
	print >> sys.stderr
	print >> sys.stderr, '#include "html.h"'
	print >> sys.stderr
	for n, ident in enumerate (idents):
		print >> sys.stderr, "const Page page%02d PROGMEM = {%s_name, %s, NULL};" % (n + 1, ident, ident)

	print >> sys.stderr

	print >> sys.stderr, "const Page* const pages[] PROGMEM = {"
	for n in xrange (1, n_pages + 1):
		print >> sys.stderr, "\t&page%02d," % n

	print >> sys.stderr, "\tNULL"
	print >> sys.stderr, "};"
