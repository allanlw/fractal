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
#ifndef _METADATA_H
#define _METADATA_H

#include <string>
#include <istream>
#include <ostream>

class MetaData {
private:
	int width;
	int height;
	std::string sourceFilename;
public:
	MetaData();
	MetaData(std::istream& in);

	int getWidth() const;
	void setWidth(int width);
	int getHeight() const;
	void setHeight(int height);
	std::string getSourceFilename() const;
	void setSourceFilename(std::string filename);

	MetaData& operator=(const MetaData& other);

	void serialize(std::ostream& out) const;
};

#endif
