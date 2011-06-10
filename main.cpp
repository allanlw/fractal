#include <cstdio>
#include <iostream>
#include <fstream>
#include "gd.h"

#include "doubleimage.hpp"
#include "triangletree.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	FILE* lennaHandle = fopen("lenna.png", "r");
	if (lennaHandle == NULL) {
		cout << "Could not load lenna.png, dying." << endl;
		return 1;
	}
	gdImagePtr lenna = gdImageCreateFromPng(lennaHandle);
	fclose(lennaHandle);

	DoubleImage image(lenna);
	TriangleTree tree(image);

	for (int i = 0; true; i++) {
		Triangle* cur = tree.assignOne();
		if (cur == NULL) break;
		cout << tree.getLastId();
		cout << " - "<< (cur->isTerminal()?"Terminal":"Not Terminal") << endl;
		if (cur->isTerminal()) {
			cout << " Error: " << cur->getTarget().error << endl;
		}
	}

	ofstream out("fractal.bin", ios_base::out | ios_base::trunc | ios_base::binary);
	cout << "Dumping Image..." << endl;
	tree.serialize(out);
	cout << "Image dumped." << endl;

	FILE* noiseHandle = fopen("noise.png", "r");
	if (noiseHandle == NULL) {
		cout << "Could not load noise.png, dying." << endl;
		return 1;
	}
	gdImagePtr noise = gdImageCreateFromPng(noiseHandle);
	fclose (noiseHandle);

	image.setImage(noise);

	tree.eval();
	gdImagePng(image.getImage(), fopen("e1.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e2.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e3.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e4.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e5.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e6.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e7.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e8.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e9.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e10.png", "w"));
	tree.eval();
	gdImagePng(image.getImage(), fopen("e11.png", "w"));
	return 0;
}
