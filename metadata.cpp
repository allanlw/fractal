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
#include "metadata.h"

#include "ioutils.h"

using namespace std;

MetaData::MetaData() {
	width = 0;
	height = 0;
	sourceFilename = "";
}

MetaData::MetaData(istream& in) {
	width = unserializeSignedInt(in);
	height = unserializeSignedInt(in);
	sourceFilename = unserializeString(in);
}

MetaData& MetaData::operator=(const MetaData& other) {
	if (&other != this) {
		this->width = other.width;
		this->height = other.height;
		this->sourceFilename = other.sourceFilename;
	}
	return *this;
}

int MetaData::getWidth() const {
	return width;
}

void MetaData::setWidth(int width) {
	this->width = width;
}

int MetaData::getHeight() const {
	return height;
}

void MetaData::setHeight(int height) {
	this->height = height;
}

string MetaData::getSourceFilename() const {
	return sourceFilename;
}

void MetaData::setSourceFilename(string filename) {
	this->sourceFilename = filename;
}

void MetaData::serialize(ostream& out) const {
	serializeSignedInt(out, width);
	serializeSignedInt(out, height);
	serializeString(out, sourceFilename);
}
