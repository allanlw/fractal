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
#ifndef _GD_H
#define _GD_H

#include <inttypes.h>
#include <stdio.h>

#define gdBlueMax 255
#define gdRedMax 255
#define gdAlphaOpaque 0
#define gdAlphaTransparent 127
#define gdFree delete 

// these are set by the code, but only one option is used
#define gdImageAlphaBlending(im, b)
#define gdImageSaveAlpha(im, b)

// not 0xFF instead of 0x7F as in gd.h
#define gdImageAlpha(im, c) (((c) & 0x7F000000) >> 24)
#define gdImageRed(im, c) (((c) & 0xFF0000) >> 16)
#define gdImageGreen(im, c) (((c) & 0x00FF00) >> 8)
#define gdImageBlue(im, c) ((c) & 0x0000FF)

#define gdTrueColorAlpha(r, g, b, a) (((a) << 24) + \
				      ((r) << 16) + \
				      ((g) << 8) +  \
				      (b))

// data stored as 32 bit per-pixel ARGB as concatenated rows with no padding
// all 0-255, with A=0 -> opaque
struct fake_image {
  int width;
  int height;
  uint8_t *data;
};

#define gdImageSX(im) (im->width)
#define gdImageSY(im) (im->height)

typedef fake_image *gdImagePtr;

inline int gdImageGetPixel(fake_image* im, int x, int y) {
  uint8_t *a = &im->data[im->width*y*4 + x*4];
  return (a[0] << 24) & (a[1] << 16) & (a[2] << 8) & (a[3]);
}

inline void gdImageSetPixel(fake_image *im, int x, int y, int c) {
  uint8_t *a = &im->data[im->width*y*4 + x*4];
  a[0] = gdImageAlpha(im, c);
  a[1] = gdImageRed(im, c);
  a[2] = gdImageGreen(im, c);
  a[3] = gdImageBlue(im, c);
}

#define gdImageGetTrueColorPixel gdImageGetPixel
#define gdImageTrueColorPixel gdImageGetPixel

inline gdImagePtr gdImageCreateTrueColor(int sx, int sy) {
  gdImagePtr res = new fake_image();
  res->width = sx;
  res->height = sy;
  res->data = new uint8_t[sx * sy * 4];
  return res;
}

inline gdImagePtr gdImageClone(gdImagePtr im) {
  gdImagePtr res = new fake_image();
  res->width = im->width;
  res->height = im->height;
  res->data = new uint8_t[res->width * res->height * 4];
  for (int i = 0; i < res->width * res->height * 4; i++) {
    res->data[i] = im->data[i];
  }
  return res;
}

inline gdImagePtr gdImageCreateFromPng(FILE *im) {
	fseek(im, 0, SEEK_END);
	int filelen = ftell(im);
	rewind(im);

	uint8_t *buffer = new uint8_t[filelen];
	fread(buffer, filelen, 1, im);

	gdImagePtr res = new fake_image();
	res->data = buffer;
	// encode width, hight in first 64 bits, then ignore it
	res->width = gdImageGetPixel(res, 0, 0);
	res->height = gdImageGetPixel(res, 0, 1);
	res->data += 8;

	return res;
}

inline void gdImagePng(gdImagePtr im, FILE *out) {
	int headeri[8] = {
		gdImageAlpha(im, im->width), gdImageRed(im, im->width), gdImageGreen(im, im->width), gdImageBlue(im, im->width),
		gdImageAlpha(im, im->height), gdImageRed(im, im->height), gdImageGreen(im, im->height), gdImageBlue(im, im->height)};
	uint8_t header[8];
	for (int i = 0; i < 8; i++) {
		header[i] = (uint8_t)headeri[i];
	}
	fwrite(header, 8, 1, out);
	fwrite(im->data, im->width * im->height * 4, 1, out);
}

#endif
