/*
 * Copyright (c) 2011 Allan Wirth <allanlw@gmail.com>
 *
 * This file is part of Fractal Image Compressor.
 *
 * Fractal Image Compressor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fractal Image Compressor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fractal Image Compressor.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _CONSTANT_HPP
#define _CONSTANT_HPP

// Default options that can be changed at runtime

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

#ifndef DEFAULT_SUBDIVISION_METHOD
#define DEFAULT_SUBDIVISION_METHOD TriangleTree::M_QUAD
#endif

#ifndef DEFAULT_EDGE_DETECTION_METHOD
#define DEFAULT_EDGE_DETECTION_METHOD DoubleImage::M_LAPLACE
#endif

// Compile time only options

#ifndef SAME_SIDE_TECHNIQUE
#define SAME_SIDE_TECHNIQUE false
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
#define PIXELS_FOR_INTERP 2
#endif

#ifndef MAX_SUBDIVIDE_SIZE
#define MAX_SUBDIVIDE_SIZE 20
#endif

#ifndef MAX_SUBDIVIDE_AREA
#define MAX_SUBDIVIDE_AREA (250./(512 * 512))
#endif

#ifndef MAX_NUM_TRIANGLES
#define MAX_NUM_TRIANGLES 0xFFFF
#endif

#endif
