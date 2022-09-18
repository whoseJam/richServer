#pragma once

#include<string>

int Rand(int l,int r);
int nxt(int x,int t);
bool readItem(FILE* file,std::string& ans);
int stringToInt(std::string& ans);
std::string intToString(int a);