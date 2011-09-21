LDFLAGS = -lm -lgd

# -msse2 does NOT inline calls to cos,tan, etc. is this bad?

# FLAGS_ARCH = -msse -msse2 -mfpmath=sse
FLAGS_ARCH = -march=native
# FLAGS_MATH = -ffast-math
# FLAGS_DEBUG = -ggdb -Wextra
# FLAGS_PROF = -pg
FLAGS_OPT = -O
FLAGS_STD = -std=c++0x
FLAGS_WARN = -Wall -Wextra

CXXFLAGS = $(FLAGS_ARCH) $(FLAGS_MATH) $(FLAGS_OPT) $(FLAGS_DEBUG)
CXXFLAGS += $(FLAGS_PROF) $(FLAGS_STD) $(FLAGS_WARN)
CPPSOURCES = $(wildcard *.cpp)
CPPHEADERS = $(wildcard *.h)

OBJS = $(patsubst %.cpp, %.o, $(CPPSOURCES))

CXX = g++

all: fractal

# yeah I know I know this could cause extra builds but who cares.
%.o: %.cpp $(CPPHEADERS)
	$(CXX) -c $(CXXFLAGS) $<

fractal: $(OBJS) $(CPPHEADERS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o fractal $(OBJS)

clean:
	rm -f fractal $(OBJS)
