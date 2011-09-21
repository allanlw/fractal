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
#ifndef _FRACTALIMAGE_H
#define _FRACTALIMAGE_H

#include <vector>
#include <istream>
#include <ostream>
#include "gd.h"

#include "triangletree.h"
#include "doubleimage.h"
#include "triangle.h"
#include "metadata.h"

class FractalImage {
public:
	enum ImageType {
		T_GREYSCALE,
		T_COLOR
	};
private:
	ImageType type;
	DoubleImage image;
	std::vector<TriangleTree*> channels;
	MetaData metadata;
public:
	FractalImage(std::istream& in, DoubleImage image);
	FractalImage(DoubleImage image, ImageType type);
	const DoubleImage& getImage() const;
	void setImage(DoubleImage image);
	ImageType getType() const;
	std::vector<Triangle*>::size_type getSize() const;
	const std::vector<TriangleTree*>& getChannels() const;
	MetaData& getMetadata();
	void serialize(std::ostream& out) const;
	void encode(double error);
	void setSubdivisionMethod(TriangleTree::SubdivisionMethod sMethod);
	gdImagePtr decode(bool fixErrors);
	~FractalImage();
};

#endif
