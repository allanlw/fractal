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
