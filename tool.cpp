#pragma once

#include<cstdlib>

#include"tool.h"

int Rand(int l, int r) {
	return rand()%(r-l+1)+l;
}

int nxt(int x, int t) {
	return x==t?1:x+1;
}

bool readItem(FILE* file,std::string& ans) {
	char c=fgetc(file);
	while (c == ' ' || c == '\n' || c == ',') c=fgetc(file);
	if(c==EOF)return false;
	while (c != ' ' && c != '\n' && c != ',') {
		ans.push_back(c);
		c=fgetc(file);
		if(c==EOF)break;
	}return true;
}

int stringToInt(std::string& str) {
	int ans=0;
	for (int i = 0; i < str.length(); i++) {
		ans=ans*10+str[i]-'0';
	}return ans;
}

std::string intToString(int a) {
	if(a==0)return "0";
	std::string ans;bool flg=false;
	if(a<0)flg=true , a=-a;
	while (a) { ans.push_back(a % 10 + '0'); a /= 10; }
	for(int l=0,r=ans.length()-1;l<=r;l++,r--)
		std::swap(ans[l],ans[r]);
	if(flg)ans="-"+ans;
	return ans;
}