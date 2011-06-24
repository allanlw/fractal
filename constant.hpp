#ifndef _CONSTANT_HPP
#define _CONSTANT_HPP

#ifndef DEFAULT_ENC_FNAME
#define DEFAULT_ENC_FNAME "fractal.bin"
#endif

#ifndef DEFAULT_DEC_FNAME
#define DEFAULT_DEC_FNAME "fractal.png"
#endif

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 256
#endif

#ifndef DEFAULT_WIDTH
#ifdef DEFAULT_HEIGHT
#define DEFAULT_WIDTH DEFAULT_HEIGHT
#else
#define DEFAULT_WIDTH DEFAULT_SIZE
#endif
#endif

#ifndef DEFAULT_HEIGHT
#ifdef DEFAULT_WIDTH
#define DEFAULT_HEIGHT DEFAULT_WIDTH
#else
#define DEFAULT_HEIGHT DEFAULT_SIZE
#endif
#endif

#ifndef DEFAULT_ITERATIONS
#define DEFAULT_ITERATIONS 20
#endif

#ifndef DEFAULT_ERROR_CUTOFF
#define DEFAULT_ERROR_CUTOFF 10
#endif

#ifndef DEFAULT_SAMPLING_TYPE
#define DEFAULT_SAMPLING_TYPE DoubleImage::T_SUBSAMPLE
#endif

#ifndef DEFAULT_FIX_ERRORS
#define DEFAULT_FIX_ERRORS true
#endif

#ifndef DEFAULT_OUTPUT_LEVEL
#define DEFAULT_OUTPUT_LEVEL 1
#endif

#ifndef DEFAULT_OUTPUT_STREAM
#define DEFAULT_OUTPUT_STREAM std::cout
#endif

#ifndef DEFAULT_COLOR_MODE
#define DEFAULT_COLOR_MODE FractalImage::T_GREYSCALE
#endif

#ifndef DEFAULT_DIVISION_TYPE
#define DEFAULT_DIVISION_TYPE DoubleImage::T_LOWENTROPY
#endif

#ifndef DEFAULT_METRIC
#define DEFAULT_METRIC DoubleImage::M_RMS
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

#ifndef ERROR_COLOR
#define ERROR_COLOR gdTrueColorAlpha(gdRedMax, 0, gdBlueMax, gdAlphaOpaque)
#endif

#ifndef PIXELS_FOR_INTERP
#define PIXELS_FOR_INTERP 3
#endif

#endif
