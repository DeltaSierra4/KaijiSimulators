# Makefile for files used in the LimitedRPS executable
# Requires clang++ 6.0 or above to run
#
# make Contestant: compiles and creates Contestant.o
# make all:				 compiles and creates LimitedRPS executable
#
# Written by Vincent Yang, 2018/11/23

EXE = LimitedRPS
OBJS_DIR = .objs
OBJS_ALL = LimitedRPS.o Contestant.o
WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -g -O0 $(WARNINGS) -MMD -MP -c
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lpthread

all: $(EXE)

# Pattern rules for object files
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
		$(CXX) $(CXXFLAGS) $< -o $@

# Create directories
$(OBJS_DIR):
		@mkdir -p $(OBJS_DIR)

# Rules for executable
$(EXE):
		$(LD) $^ $(LDFLAGS) -o $@

# Executable dependencies
$(EXE):      $(patsubst %.o, $(OBJS_DIR)/%.o,      $(OBJS_ALL)) #$(patsubst %.o, $(OBJS_DIR)/%.o)

# Include automatically generated dependencies
-include $(OBJS_DIR)/*.d

LimitedRPS.o: LimitedRPS.cpp
		$(CXX) $(CXXFLAGS) LimitedRPS.cpp

Contestant.o: Contestant.cpp Contestant.h
		$(CXX) $(CXXFLAGS) Contestant.cpp

clean:
		rm -rf $(EXE) $(EXE)-asan $(OBJS_DIR) test tests/*.d tests/*.o *.d
