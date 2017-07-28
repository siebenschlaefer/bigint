# needs GNU make

CXX      ?= g++
CFLAGS   += -Wall -Wextra -std=c11
CXXFLAGS += -Wall -Wextra -std=c++14
CPPFLAGS += -Igoogletest
LDFLAGS  +=
LDLIBS   += -pthread

PROGRAMS = bigint_test
LIBRARIES =

all: $(LIBRARIES) $(PROGRAMS)

check: all
	./bigint_test

#example: example.o obj2.o
bigint_test: bigint_test.o bigint.o \
	./googletest/gmock-gtest-all.o ./googletest/gmock_main.o

$(LIBRARIES): %.a:
	ar -rv $@ $^
	ranlib $@

$(PROGRAMS): %: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.C
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%: %.rl
	ragel -o $@ $<

.PHONY: all check clean
clean:
	rm -f $(PROGRAMS) core *.o .*.dep

.%.dep: %.c
	@$(CC)  $(CFLAGS)   -M $(CPPFLAGS) $< > $@

.%.dep: %.cc
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

.%.dep: %.cpp
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

.%.dep: %.cxx
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

.%.dep: %.C
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

-include $(patsubst   %.c,.%.dep, $(wildcard   *.c))
-include $(patsubst  %.cc,.%.dep, $(wildcard  *.cc))
-include $(patsubst %.cpp,.%.dep, $(wildcard *.cpp))
-include $(patsubst %.cxx,.%.dep, $(wildcard *.cxx))
-include $(patsubst   %.C,.%.dep, $(wildcard   *.C))

