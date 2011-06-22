#ifndef IMAGEUTILS_HPP_
#define IMAGEUTILS_HPP_

#include "gd.h"

gdImagePtr edgeDetectSobel(const gdImagePtr image);
gdImagePtr edgeDetectLaplace(const gdImagePtr image);
gdImagePtr edgeDetect(const gdImagePtr image, int op(const unsigned char*));
int wrappedGetPixel(const gdImagePtr img, int x, int y);
unsigned char getGrey(const gdImagePtr img, int c);
unsigned char getPixel(const gdImagePtr img, int x, int y, bool check=true);
void setPixel(gdImagePtr img, int x, int y, unsigned char grey);
gdImagePtr blankCanvas(int w, int h, unsigned long seed);
void interpolateErrors(gdImagePtr img);
bool interpolatePixel(gdImagePtr img, int x, int y);

inline unsigned char boundColor(int c) {
	return (c>gdRedMax)?(gdRedMax):((c<0)?0:c);
}

void clearAlpha(gdImagePtr img);

gdImagePtr loadImage(const char* fName);

#endif
