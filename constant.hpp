#ifndef CONSTANT_HPP_
#define CONSTANT_HPP_

#ifndef DEFAULT_ENC_FNAME
#define DEFAULT_ENC_FNAME "fractal.bin"
#endif

#ifndef DEFAULT_DEC_FNAME
#define DEFAULT_DEC_FNAME "fractal.png"
#endif

#ifndef DEFAULT_WIDTH
#ifdef DEFAULT_HEIGHT
#define DEFAULT_WIDTH DEFAULT_HEIGHT
#else
#define DEFAULT_WIDTH 256
#endif
#endif

#ifndef DEFAULT_HEIGHT
#ifdef DEFAULT_WIDTH
#define DEFAULT_HEIGHT DEFAULT_WIDTH
#else
#define DEFAULT_HEIGHT 256
#endif
#endif

#ifndef DEFAULT_ITERATIONS
#define DEFAULT_ITERATIONS 20
#endif

#ifndef DEFAULT_ERROR_CUTOFF
#define DEFAULT_ERROR_CUTOFF 10*10
#endif

#ifndef SAME_SIDE_TECHNIQUE
#define SAME_SIDE_TECHNIQUE false
#endif

#ifndef EDGE_DETECT_SOBEL
#define EDGE_DETECT_SOBEL true
#endif

#ifndef MIN_SUBDIVIDE_RATIO
#define MIN_SUBDIVIDE_RATIO .25
#endif

#ifndef MIN_SEARCH_RATIO
#define MIN_SEARCH_RATIO 2
#endif

#ifndef PREDICT_ACCURACY
#define PREDICT_ACCURACY 4
#endif

#ifndef USE_HEURISTICS
#define USE_HEURISTICS 0
#endif

#endif
