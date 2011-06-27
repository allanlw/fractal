LDFLAGS = -lm -lgd
STDFLAGS = -Wall -std=c++0x

# -msse2 does NOT inline calls to cos,tan, etc. is this bad?

# FLAGS_ARCH = -msse -msse2 -mfpmath=sse
FLAGS_ARCH = -march=native
# FLAGS_MATH = -ffast-math
# FLAGS_DEBUG = -ggdb -Wextra
# FLAGS_PROF = -pg

CXXFLAGS = $(FLAGS_ARCH) $(FLAGS_MATH) -O $(STDFLAGS) $(FLAGS_DEBUG) $(FLAGS_PROF)

CPPSOURCES = $(wildcard *.cpp)
CPPHEADERS = $(wildcard *.h)

OBJS = $(patsubst %.cpp, %.o, $(CPPSOURCES))

CXX = g++

all: fractal

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $+

fractal: $(OBJS) $(CPPHEADERS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o fractal $(OBJS)

clean:
	rm -f fractal $(OBJS)
