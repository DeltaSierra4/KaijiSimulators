# Makefile for files used in the OnePokerSim executable.
# Requires clang++ 6.0 or above to run.
#
# make OPContestant: compiles and creates OPContestant.o
# make all:				   compiles and creates OnePokerSim executable
#
# Written by Vincent Yang, 2018/12/30

EXE = OnePokerSim
OBJS_DIR = .objs
OBJS_ALL = OnePokerSim.o OPContestant.o PokerCards.o
WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libstdc++ -g -O0 $(WARNINGS) -MMD -MP -c
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libstdc++ -lpthread #-lc++abi

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

OPContestant.o: OPContestant.cpp OPContestant.h
		$(CXX) $(CXXFLAGS) OPContestant.cpp

clean:
		rm -rf $(EXE) $(EXE)-asan $(OBJS_DIR) test tests/*.d tests/*.o
