#
# Makefile for LKit tests
#
# We need to have a few system-level dependencies to make things cleanly
# compile on a few different systems.
#
ifeq ($(shell uname),SunOS)
CXX = g++
CXX_DEFS = -g -O3
LIB_EXT = so
ifeq ($(shell uname -r),5.6)
OS_LIBS = -lsocket -lnsl -lposix4
else
OS_LIBS = -lsocket -lnsl -lrt
endif
SHARED = -shared
endif
ifeq ($(shell uname),Darwin)
CXX = g++
CXX_DEFS = -D_REENTRANT -g -O3 -arch i386
LIB_EXT = dylib
OS_LIBS =
SHARED = -dynamiclib
endif
ifeq ($(shell uname),Linux)
CXX = g++
CXX_DEFS = -g -O3
LIB_EXT = so
OS_LIBS = -lpthread -lrt
SHARED = -shared
endif

#
# These are the locations of certain directories in the package
#
LIB_DIR = ../lib
SRC_DIR = ../src

#
# This is the ultimate target we're making - the library with the entire
# LKit contained within it.
#
LIB_FILE = $(LIB_DIR)/libLKit.$(LIB_EXT)

#
# These are the pretty standard C++-style defines for a makefile - the
# includes, the libs, the compiler flags, etc.
#
INCLUDES = -I. -I$(SRC_DIR)
DEFINES = $(CXX_DEFS)
CXXFLAGS = -fPIC -Wall $(INCLUDES) $(DEFINES)

LIBS = -L$(LIB_DIR) $(OS_LIBS) -lboost_thread -lboost_system -lstdc++ -lLKit
LDFLAGS =

#
# These are the main targets that we'll be making
#
APPS = value expression parser timer
SRCS = $(APPS:%=%.cpp)

all: $(APPS)

clean:
	rm -f $(APPS)
	rm -rf *.dSYM

tests: all
	@ echo '========= Value Tests ========='
	@ value

depend:
	makedepend -Y -o\  -- $(INCLUDES) -- $(SRCS); rm Makefile.bak

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

expression: expression.cpp $(LIB_FILE)
	$(CXX) $(CXXFLAGS) expression.cpp -o expression $(LIBS) $(LDFLAGS)

parser: parser.cpp $(LIB_FILE)
	$(CXX) $(CXXFLAGS) parser.cpp -o parser $(LIBS) $(LDFLAGS)

timer: timer.cpp $(LIB_FILE)
	$(CXX) $(CXXFLAGS) timer.cpp -o timer $(LIBS) $(LDFLAGS)

value: value.cpp $(LIB_FILE)
	$(CXX) $(CXXFLAGS) value.cpp -o value $(LIBS) $(LDFLAGS)

# DO NOT DELETE

value : ../src/value.h ../src/util/timer.h
expression : ../src/value.h ../src/base_functions.h ../src/function.h
expression : ../src/expression.h ../src/util/timer.h
parser : ../src/parser.h ../src/variable.h ../src/value.h ../src/util/timer.h
timer : ../src/util/timer.h
