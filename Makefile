LDFLAGS = -lm -lgd
CXXFLAGS = -Wall

# -msse2 does NOT inline calls to cos,tan, etc. is this bad?

FLAGS_OPT = -msse -msse2 -mfpmath=sse -ffast-math -fno-finite-math-only -O3
FLAGS_PROF = -pg $(FLAGS_OPT)
FLAGS_DEBUG = -g -DDO_CHECKS -Wextra
FLAGS_OPTDEBUG = $(FLAGS_OPT) -g

CPPSOURCES = $(wildcard *.cpp)
CPPHEADERS = $(wildcard *.h) $(wildcard *.hpp)

FRACTALDEPS = $(CPPSOURCES) $(CPPHEADERS)

CXX = g++

TARGET = fractal

all: fractal-opt

opt: fractal-opt

debug: fractal-debug

prof: fractal-prof

opt-debug: fractal-opt-debug

fractal-opt: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_OPT) -o $(TARGET) $(CPPSOURCES)

fractal-debug: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_DEBUG) -o $(TARGET) $(CPPSOURCES)

fractal-prof: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_PROF) -o $(TARGET) $(CPPSOURCES)

fractal-opt-debug: $(FRACTALDEPS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(FLAGS_OPTDEBUG) -o $(TARGET) $(CPPSOURCES)
