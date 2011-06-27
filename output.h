#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <string>
#include <ostream>
#include <iostream>

extern unsigned char outputLevel;

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
