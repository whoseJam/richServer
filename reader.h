#pragma once

#include<iostream>
#include<vector>

struct Reader {
	const static int MAXLEN = 1 << 15;
	char buf[MAXLEN]; int pos, l;
	void init(std::string& str);
	int readInt();
	std::string readString();
	bool isEmpty();
	void inputFile(FILE* fp);
};