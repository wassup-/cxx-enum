# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
AS=as

# Compilers
CC=/opt/gcc-4.8.1/bin/gcc
CXX=/opt/gcc-4.8.1/bin/g++

# Compiler flags
LDFLAGS=
CCFLAGS=-c -std=c++0x -Wall -O3
CXXFLAGS=-c -std=c++0x -Wall -O3

# Directories
BINDIR=bin
INCLUDEDIR=include
SRCDIR=src

EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)

FILES=		example.cpp

SOURCES=$(SRCDIR)/$(subst $(SPACE), $(SRCDIR)/,$(FILES))
OBJECTS=$(subst .cpp,.o,$(BINDIR)/$(subst $(SPACE), $(BINDIR)/,$(FILES)))
SRCTOBIN=$(subst .cpp,.o,$(subst $(SRCDIR),$(BINDIR),$(1)))
BINTOSRC=$(subst .o,.cpp,$(subst $(BINDIR),$(SRCDIR),$(1)))

EXECUTABLE:=example

# Stages

all: $(SOURCES) $(EXECUTABLE)

$(SOURCES):
	$(MKDIR) -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $@ -o $(call SRCTOBIN,$@)

$(EXECUTABLE): $(OBJECTS)
	$(MKDIR) -p $(BINDIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJECTS):
	$(MKDIR) -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(call BINTOSRC,$@) -o $@

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(BINDIR)/
	rm -rf $(EXECUTABLE)