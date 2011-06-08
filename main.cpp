#include <cstdio>
#include <iostream>
#include "gd.h"

#include "doubleimage.hpp"
#include "triangletree.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	FILE* lennaHandle = fopen("lenna.png", "r");
	if (lennaHandle == NULL) {
		cout << "Could not load lenna.png, dying." << endl;
	}
	gdImagePtr lenna = gdImageCreateFromPng(lennaHandle);
	fclose(lennaHandle);
	DoubleImage image(lenna);
	TriangleTree tree(image);

	for (int i = 0; i < 10; i++) {
		Triangle* cur = tree.assignOne();
		if (cur == NULL) break;
		cout << tree.getLastId();
		cout << " - "<< (cur->isTerminal()?"Termainal":"Not Termainal");
		if (cur->isTerminal()) {
			cout << " Error: " << cur->getTarget().error;
		}
		cout << endl;
	}
	return 0;
}
