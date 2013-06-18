# Environment
MKDIR=MKDIR
CP=cp
GREP=grep
NM=nm
AS=as

# Compilers
CC=gcc
CXX=g++

# Compiler flags
CCFLAGS=-std=c++0x
CXXFLAGS=-std=c++11

# Directories
BINDIR=bin
INCLUDEDIR=include
SRCDIR=src

# Stages

all: cxx-enum

cxx-enum: ${SRCDIR}/example.cpp
	${MKDIR} -p ${BINDIR}

clean:
	rm -rf ${BINDIR}/*