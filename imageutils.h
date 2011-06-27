#ifndef _IMAGEUTILS_H
#define _IMAGEUTILS_H

#include "gd.h"
#include <string>

enum Channel {
	C_GREY,
	C_RED,
	C_GREEN,
	C_BLUE
};

std::string channelToString(Channel channel);

gdImagePtr edgeDetectSobel(const gdImagePtr image, Channel channel);
gdImagePtr edgeDetectLaplace(const gdImagePtr image, Channel channel);
gdImagePtr edgeDetect(const gdImagePtr image, int op(const unsigned char*), Channel channel);
int wrappedGetPixel(const gdImagePtr img, int x, int y);
unsigned char getColor(const gdImagePtr img, int c, Channel channel);
unsigned char getGrey(const gdImagePtr img, int c);
unsigned char getPixel(const gdImagePtr img, int x, int y, Channel channel, bool check=true);
void setPixel(gdImagePtr img, int x, int y, unsigned char value, Channel channel, unsigned char alpha=gdAlphaOpaque);
gdImagePtr blankCanvas(int w, int h, unsigned long seed);
void interpolateErrors(gdImagePtr img, Channel channel);
bool interpolatePixel(gdImagePtr img, int x, int y, Channel channel);

inline unsigned char boundColor(int c) {
	return (c>gdRedMax)?(gdRedMax):((c<0)?0:c);
}

void clearAlpha(gdImagePtr img);

gdImagePtr loadImage(const char* fName);

#endif
