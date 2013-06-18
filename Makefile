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

# Helpers
EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)

FILES=example.cpp

SOURCES=$(SRCDIR)/$(subst $(SPACE), $(SRCDIR)/,$(FILES))
OBJECTS=$(subst .cpp,.o,$(BINDIR)/$(subst $(SPACE), $(BINDIR)/,$(FILES)))
SOURCE=$(subst .o,.cpp,$(subst $(BINDIR),$(SRCDIR),$(1)))

EXE_PREFIX=
EXE_POSTFIX=
EXE_BASE=cxx-enum
EXECUTABLE=$(EXE_PREFIX)$(EXE_BASE)$(EXE_POSTFIX)

# Stages

all: $(EXE_BASE)

$(EXE_BASE): $(OBJECTS)
	$(MKDIR) -p $(BINDIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJECTS):
	$(MKDIR) -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(call SOURCE,$@) -o $@

clean:
	rm -rf $(BINDIR)/