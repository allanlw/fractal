LDFLAGS = -lm -lgd
TESTLDFLAGS = -lcppunit
CXXFLAGS = -Wall -std=c++0x

# -msse2 does NOT inline calls to cos,tan, etc. is this bad?

# FLAGS_SSE = -msse -msse2 -mfpmath=sse
FLAGS_SSE = -march=native
# FLAGS_MATH = -ffast-math

FLAGS_OPT = $(FLAGS_SSE) $(FLAGS_MATH) -O
FLAGS_PROF = -pg $(FLAGS_OPT)
FLAGS_DEBUG = -ggdb -Wextra
FLAGS_OPTDEBUG = $(FLAGS_OPT) -g

TESTSOURCES = $(wildcard *test.cpp)
TESTHEADERS = $(wildcard *test.hpp) $(wildcard *test.h)

CPPSOURCES = $(filter-out $(TESTSOURCES), $(wildcard *.cpp))
CPPHEADERS = $(filter-out $(TESTHEADERS), $(wildcard *.h) $(wildcard *.hpp))

FRACTALDEPS = $(CPPSOURCES) $(CPPHEADERS)

CXX = g++

TARGET = fractal

all: fractal-opt

opt: fractal-opt

debug: fractal-debug

prof: fractal-prof

test: $(TESTSOURCES) $(TESTHEADERS) $(CPPSOURCES) $(CPPHEADERS)
	$(CXX) $(LDFLAGS) $(TESTLDFLAGS) $(CXXFLAGS) $(FLAGS_DEBUG) -o test $(TESTSOURCES) $(filter-out main.cpp, $(CPPSOURCES))

opt-debug: fractal-opt-debug

fractal-opt: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_OPT) -o $(TARGET) $(CPPSOURCES)

fractal-debug: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_DEBUG) -o $(TARGET) $(CPPSOURCES)

fractal-prof: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_PROF) -o $(TARGET) $(CPPSOURCES)

fractal-opt-debug: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_OPTDEBUG) -o $(TARGET) $(CPPSOURCES)
