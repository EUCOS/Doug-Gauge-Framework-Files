LIBSRC=src src/Plugins
LIBRARY=$(LIBDIR)/libgauge.a

CMDSRC=$(wildcard cmd/*)

GTSOURCES=$(GTSRCDIR)/gtest-all.cc $(GTSRCDIR)/gtest_main.cc
GTLIB=$(TESTLIBDIR)/libgtest.a

BINDIR:=bin
OBJDIR:=obj
INCDIR:=include
LIBDIR:=lib

TESTDIR:=test
TESTBINDIR:=$(TESTDIR)/bin
TESTSRCDIR:=$(TESTDIR)/src
TESTINCDIR:=$(TESTDIR)/include
TESTLIBDIR:=$(TESTDIR)/lib
GTROOT:=$(TESTDIR)/gtest
GTINCDIR:=$(GTROOT)/include
GTOBJDIR:=$(GTROOT)/obj
GTSRCDIR:=$(GTROOT)/src

ARCHIVER:=ar rcs
COMPILER:=g++ -c
LINKER:=g++

LIBS+=-lm -lpthread
FLAGS+=-Wall -ansi

# Derived Library Variables
LIBSOURCES=$(wildcard $(addsuffix /*.cpp,$(LIBSRC)))
LIBOBJ=$(subst src,$(OBJDIR),$(LIBSOURCES:%.cpp=%.o))

# Derived Executable Variables
CMDSOURCES=$(addsuffix /main.cpp,$(CMDSRC))
CMDOBJ=$(CMDSOURCES:%.cpp=%.o)
CMDBIN=$(subst cmd,$(BINDIR),$(CMDSRC))

# Derived Testing Variables
GTOBJ=$(subst src,obj,$(GTSOURCES:%.cc=%.o))
TESTSOURCES=$(wildcard $(addsuffix /*Test.cpp,$(TESTSRCDIR)))
TESTBIN=$(subst $(TESTSRCDIR),$(TESTBINDIR),$(TESTSOURCES:%.cpp=%))

all: FLAGS:=-Wall -ansi -pedantic -g -pg
all: $(LIBRARY)

release: FLAGS:=-Wall -ansi -pedantic -O3
release: $(CMDBIN)

exec: FLAGS:=-Wall -ansi -pedantic -g -pg
exec: $(CMDBIN)

# Archiving, Compiling and Cleaning the library

$(LIBRARY): $(LIBOBJ)
	@echo "Creating the Gauge Library"
	@mkdir -p $(@D)
	@$(ARCHIVER) $(LIBRARY) $(LIBOBJ)

obj/%.o: src/%.cpp
	@echo "Compiling ($(FLAGS)) $@"
	@mkdir -p $(@D)
	@$(COMPILER) -I$(INCDIR) $(FLAGS) -o $@ $<

clean-lib:
	@echo "Cleaning Gauge Framework objects and static libraries"
	@rm -f $(LIBRARY) $(LIBOBJ)

# Linking, Compiling and Cleaning the executables

$(BINDIR)/%: LIBS+=-lgauge
$(BINDIR)/%: cmd/%/main.cpp $(LIBRARY)
	@echo "Compiling and linking ($(FLAGS)) $@"
	@mkdir -p $(BINDIR)
	@$(LINKER) -I$(INCDIR) -L$(LIBDIR) $(FLAGS) -o $@ $< $(LIBS)

clean-exec:
	@echo "Cleaning command object and binary files"
	@rm -f $(CMDBIN)

# Google Testing Framework
$(GTLIB): FLAGS+=-g
$(GTLIB): $(GTOBJ)
	@echo "Creating the Google Test Library"
	@mkdir -p $(@D)
	@$(ARCHIVER) $(GTLIB) $(GTOBJ)

$(GTROOT)/obj/%.o: $(GTROOT)/src/%.cc
	@echo "Compiling ($(FLAGS)) $@"
	@mkdir -p $(@D)
	@$(COMPILER) -I$(GTINCDIR) -I$(GTROOT) $(FLAGS) -o $@ $<

clean-testlib:
	@echo "Cleaning Google Test Framework objects and static libraries"
	@rm -f $(GTLIB) $(GTOBJ)

# Linking, Compiling and Cleaning the tests
$(TESTBINDIR)/%: FLAGS+=-g
$(TESTBINDIR)/%: LIBS+=-lgauge -lgtest
$(TESTBINDIR)/%: $(TESTSRCDIR)/%.cpp $(LIBRARY) $(GTLIB)
	@echo "Compiling ($(FLAGS)) $@"
	@mkdir -p $(TESTBINDIR)
	@$(LINKER) -I$(INCDIR) -I$(TESTINCDIR) -I$(GTINCDIR) -L$(LIBDIR)\
		-L$(TESTLIBDIR) $(FLAGS) -o $(TESTBINDIR)/$* $< $(LIBS)

clean-test:
	@echo "Cleaning test object and binary files"
	@rm -f $(TESTBIN)

check: $(TESTBIN) $(addsuffix .run,$(TESTBIN))

tests: $(TESTBIN)

$(TESTBINDIR)/%.run:
	@echo "Running Test: $*"
	@./$(TESTBINDIR)/$*

clean: clean-lib clean-exec clean-test

# Making Documentation
doc:
	@doxygen
	@make -C share/latex

clean-doc:
	@echo "Cleaning documentation"
	@rm -rf share

.PHONY: clean clean-lib clean-exec clean-test clean-testlib check
