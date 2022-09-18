
#include"reader.h"

bool Reader::isEmpty() {
	return pos >= l;
}

void Reader::init(std::string& str) {
	memcpy(buf,str.c_str(),sizeof(char)*str.length());
	l = str.length();
	buf[l]='\0';
	pos = 0;
}

int Reader::readInt() {
	auto condition = [](char c) {return c < '0' || c>'9'; };
	auto skipCondition = [](char c) {return c == ',' || c == '\n'; };
	while (pos < l && condition(buf[pos]))pos++;
	int ans = 0;
	while (pos < l && !condition(buf[pos])) { ans = ans * 10 + buf[pos] - '0'; pos++; }
	while (pos < l && skipCondition(buf[pos]))pos++;
	return ans;
}

std::string Reader::readString() {
	auto condition = [](char c) {return c == ',' || c == '\n'; };
	auto skipCondition = [](char c) {return c == ',' || c == '\n'; };
	std::string ans;
	while (pos < l && condition(buf[pos]))pos++;
	while (pos < l && !condition(buf[pos])) { ans.push_back(buf[pos]); pos++; }
	while (pos < l && skipCondition(buf[pos]))pos++;
	return ans;
}

void Reader::inputFile(FILE* fp) {
	l = 0; char c = fgetc(fp);
	while (c != EOF) { buf[l++] = c; c = fgetc(fp); }
	buf[l] = '\0';
}