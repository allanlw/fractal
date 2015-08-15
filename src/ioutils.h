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
#ifndef _IOUTILS_H
#define _IOUTILS_H

#include <ostream>
#include <istream>
#include <string>

void serializeDouble(std::ostream& out, double d);
void serializeFraction(std::ostream& out, double d, int min, int max);
void serializeUnsignedShort(std::ostream& out, unsigned short s);
void serializeSignedInt(std::ostream& out, signed int i);
void serializeString(std::ostream& out, std::string str);

double unserializeDouble(std::istream& in);
double unserializeFraction(std::istream& in, int min, int max);
unsigned short unserializeUnsignedShort(std::istream& in);
signed int unserializeSignedInt(std::istream& in);
std::string unserializeString(std::istream& in);

std::string getBasename(std::string filename);

#endif
