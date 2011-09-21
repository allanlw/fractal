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
#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <string>
#include <ostream>
#include <iostream>

extern signed char outputLevel;

extern std::ostream& output;

#define OUTPUT_ERROR 0
#define OUTPUT_STD 1
#define OUTPUT_VERBOSE 2
#define OUTPUT_DEBUG 3

#define outputDebug() (outputLevel >= OUTPUT_DEBUG)
#define outputVerbose() (outputLevel >= OUTPUT_VERBOSE)
#define outputStd() (outputLevel >= OUTPUT_STD)
#define outputError() (outputLevel >= OUTPUT_ERROR)

inline void openError(const char* fName, const char* reason = NULL) {
	if (outputError()) {
		output << "Could not open " << fName;
		if (reason != NULL) {
			output << " (" << reason << ")";
		}
		output << "." << std::endl;
	}
}

inline void openError(const char* fName, std::string reason) {
	openError(fName, reason.c_str());
}

inline void openError(std::string fName) {
	openError(fName.c_str());
}

inline void openError(std::string fName, std::string reason) {
	openError(fName.c_str(), reason.c_str());
}

#endif
