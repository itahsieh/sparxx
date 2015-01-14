GTEST_DIR=../gtest
USER_DIR=../src

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -I$(USER_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -std=c++11 -g -W -Wall -Wextra -Werror -pthread

# Link flags
LNKFLAGS += -lpthread -L$(GTEST_DIR)/lib -lgtest -lgtest_main

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = model_unittest lamdaMolec_unittest

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) *.o

model_unittest : model_unittest.o model.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LNKFLAGS)
	./$@

lamdaMolec_unittest : lamdaMolec_unittest.o lamdaMolec.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LNKFLAGS)
	./$@

model.o: $(USER_DIR)/sparxModel.cc $(USER_DIR)/sparxModel.h Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

lamdaMolec.o: $(USER_DIR)/lamdaMolec.cc $(USER_DIR)/lamdaMolec.h Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

model_unittest.o: model_unittest.cc $(USER_DIR)/sparxModel.h Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

lamdaMolec_unittest.o: lamdaMolec_unittest.cc $(USER_DIR)/lamdaMolec.h Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<
