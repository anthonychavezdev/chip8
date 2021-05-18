CC = gcc
C_COMPAT    = -Wpedantic -std=c99
LIB_FLAGS   = -lSDL2
WARNINGS    = -Wall -Werror
DEBUG_FLAGS = -g
CFLAGS =  $(C_COMPAT) $(LIB_FLAGS) $(WARNINGS) $(DEBUG_FLAGS)

HEADERS = $(wildcard *.h)
OBJECTS = $(HEADERS:%.h=%.o)

chip8: chip8.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

##
# Code to check for `#include' statements.
##
DEPDIR := .dep
$(DEPDIR): ; mkdir $@
.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: %.c | $(DEPDIR)
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< -MF $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o: .dep/%.d
DEPFILES := $(OBJECTS:%.o=$(DEPDIR)/%.d)
include $(DEPFILES)

clean:
	rm -rf *.o chip8
