# This file was automatically generated by bakefile.
#
# Any manual changes will be lost if it is regenerated,
# modify the source .bkl file instead if possible.

# You may define standard make variables such as CFLAGS or
# CXXFLAGS to affect the build. For example, you could use:
#
#      make CXXFLAGS=-g
#
# to build with debug information. The full list of variables
# that can be used by this makefile is:
# AR, CC, CFLAGS, CPPFLAGS, CXX, CXXFLAGS, LD, LDFLAGS, MAKE, RANLIB.

# You may also specify config=Debug|Release
# or their corresponding lower case variants on make command line to select
# the corresponding default flags values.
ifeq ($(config),debug)
override config := Debug
endif
ifeq ($(config),release)
override config := Release
endif
ifeq ($(config),Debug)
CPPFLAGS ?= -DDEBUG
CFLAGS ?= -g -O0
CXXFLAGS ?= -g -O0
LDFLAGS ?= -g
else ifeq ($(config),Release)
CPPFLAGS ?= -DNDEBUG
CFLAGS ?= -O2
CXXFLAGS ?= -O2
else ifneq (,$(config))
$(warning Unknown configuration "$(config)")
endif

# Use "make RANLIB=''" for platforms without ranlib.
RANLIB ?= ranlib

CC := cc
CXX := c++

# The directory for the build files, may be overridden on make command line.
builddir = .

all: $(builddir)/xyg

$(builddir)/xyg: $(builddir)/xyg_binds.o $(builddir)/xyg_fileio.o $(builddir)/xyg_global.o $(builddir)/xyg_graphics.o $(builddir)/xyg_input.o $(builddir)/xyg_main.o $(builddir)/xyg_maths.o $(builddir)/xyg_sprite.o $(builddir)/xyg_structs.o $(builddir)/xyg_xyDot.o $(builddir)/xyg_xyShape.o
	$(CXX) -o $@ $(LDFLAGS) $(builddir)/xyg_binds.o $(builddir)/xyg_fileio.o $(builddir)/xyg_global.o $(builddir)/xyg_graphics.o $(builddir)/xyg_input.o $(builddir)/xyg_main.o $(builddir)/xyg_maths.o $(builddir)/xyg_sprite.o $(builddir)/xyg_structs.o $(builddir)/xyg_xyDot.o $(builddir)/xyg_xyShape.o -pthread

$(builddir)/xyg_binds.o: binds.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread binds.cpp

$(builddir)/xyg_fileio.o: fileio.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread fileio.cpp

$(builddir)/xyg_global.o: global.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread global.cpp

$(builddir)/xyg_graphics.o: graphics.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread graphics.cpp

$(builddir)/xyg_input.o: input.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread input.cpp

$(builddir)/xyg_main.o: main.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread main.cpp

$(builddir)/xyg_maths.o: maths.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread maths.cpp

$(builddir)/xyg_sprite.o: sprite.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread sprite.cpp

$(builddir)/xyg_structs.o: structs.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread structs.cpp

$(builddir)/xyg_xyDot.o: xyDot.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread xyDot.cpp

$(builddir)/xyg_xyShape.o: xyShape.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -MD -MP -pthread xyShape.cpp

clean:
	rm -f *.o
	rm -f *.d
	rm -f $(builddir)/xyg

.PHONY: all clean

# Dependencies tracking:
-include *.d