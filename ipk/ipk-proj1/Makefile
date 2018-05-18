# Faculty of Information Technology, Brno University of Technology
# Subject: IPK (Computer Communications and Networks) - Project 1
# Description: Client/Server socket communication
# Date created: February 2017
# Author: Jan Kubica
# Login: xkubic39
# Email: xkubic39@stud.fit.vutbr.cz
# File: Makefile - code compilation
#
# Usage:
#   - compile program:        make
#   - clean binaries:         clean
#   - pack to *.tgz:          pack

# Compiler
CC=g++

# Compile arguments (-pedantic -Wall -W)
CFLAGS=-std=c++11

# Filenames and extensions
SERVER=ftrest
CLIENT=ftrestd
EXT=cpp
DOC=-doc.pdf
LOGIN=xkubic39

#Compiling
make: ftrest.cpp
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).$(EXT)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).$(EXT)

# Removing binaries
clean:
	rm $(SERVER)
	rm $(CLIENT)

# Packing
pack:
	tar -cf $(LOGIN).tar Makefile $(SERVER).$(EXT) $(CLIENT).$(EXT)
	gzip $(LOGIN).tar
	mv $(LOGIN).tar.gz $(LOGIN).tgz

