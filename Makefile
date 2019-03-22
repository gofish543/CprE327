# Version we are currently on
MAJOR		= 0
MINOR		= 1
PATCH		= 6

# Compiler and Linker
CC			= gcc
CXX			= g++

# Target binary program
TARGET		= coms327_dykhuizen_$(MAJOR).$(MINOR).$(PATCH)

# Directories
SRCDIR		= src
INCDIR		= include
BUILDDIR	= obj
TARGETDIR	= bin
RESDIR		= res
SRCEXT		= cpp
DEPEXT		= d
OBJEXT		= o

# Flags
#FLAGS	= -pipe -Wall -Wpedantic -g -ggdb3
FLAGS	= -pipe -Wall -Werror -Wpedantic -funroll-loops -O3
LIB		= -lm -lncurses -lpthread
INC		= -I$(INCDIR) -I/usr/local/include
INCDEP	= -I$(INCDIR)

# Commands
ECHO		= echo

#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# Default make
all: directories resources $(TARGET)

# Remake
remake: cleaner all

# Build just $(TARGET)
build: $(TARGET)

# Copy Resources from $(RESDIR) into $(TARGETDIR)
resources:
	@cp -r $(RESDIR)/ $(TARGETDIR)/

# Make the Directories
directories:
	@mkdir -p $(RESDIR)
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*

# Clean Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)/*

# Pull in dependency information for existing .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# Linker
$(TARGET): $(OBJECTS)
	@$(ECHO) Linking $^
	@$(CXX) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

# Compiler
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@$(ECHO) Compiling $^
	@$(CXX) $(FLAGS) $(INC) -c -o $@ $<
	@$(CXX) $(FLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

# Non-File Targets
.PHONY: all remake clean cleaner directories resources
