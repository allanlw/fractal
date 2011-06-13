#ifndef IMAGEUTILS_HPP_
#define IMAGEUTILS_HPP_

#include "gd.h"

gdImagePtr edgeDetectSobel(const gdImagePtr image);
gdImagePtr edgeDetectLaplace(const gdImagePtr image);
gdImagePtr edgeDetect(const gdImagePtr image, int op(const unsigned char*));
unsigned char getPixel(const gdImagePtr img, int x, int y);
void setPixel(gdImagePtr img, int x, int y, unsigned char grey);

inline unsigned char boundColor(int c) {
	return (c>gdRedMax)?(gdRedMax):((c<0)?0:c);
}

void clearAlpha(gdImagePtr img);

#endif
