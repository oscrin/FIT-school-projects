# Faculty of Information Technology, Brno University of Technology
# Subject: IPK (Computer Communications and Networks) - Project 2
# Description: MathClient
# Date created: April 2017
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
CFLAGS=-std=c++11 -Wall -pedantic

# Filenames and extensions
CLIENT=ipk-client
EXT=cpp
DOC=ipk-client.1
LOGIN=xkubic39

#Compiling
make: ipk-client.cpp
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).$(EXT) -lssl -lcrypto

# Removing binaries
clean:
	rm $(CLIENT)

# Packing
pack:
	tar -cf $(LOGIN).tar Makefile $(CLIENT).$(EXT) $(DOC)
	gzip $(LOGIN).tar
	mv $(LOGIN).tar.gz $(LOGIN).tgz

