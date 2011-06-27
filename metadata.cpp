#include "metadata.hpp"

#include "ioutils.hpp"

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
