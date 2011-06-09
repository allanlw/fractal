#ifndef IMAGEUTILS_HPP_
#define IMAGEUTILS_HPP_

#include "gd.h"

gdImagePtr edgeDetectSobel(gdImagePtr image);
gdImagePtr edgeDetectLaplace(gdImagePtr image);
gdImagePtr edgeDetect(gdImagePtr image, int op(const unsigned char*));

#endif
