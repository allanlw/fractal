#ifndef OUTPUT_HPP_
#define OUTPUT_HPP_

#include <ostream>

extern unsigned char outputLevel;

extern std::ostream& output;

#define OUTPUT_ERROR 0
#define OUTPUT_STD 1
#define OUTPUT_VERBOSE 2
#define OUTPUT_DEBUG 3

#define outputDebug() (outputLevel >= OUTPUT_DEBUG)
#define outputVerbose() (outputLevel >= OUTPUT_VERBOSE)
#define outputStd() (outputLevel >= OUTPUT_STD)
#define outputError() (outputLevel >= OUTPUT_ERROR)

#endif
