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
