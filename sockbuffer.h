#pragma once

#include<queue>
#include<string>
#include<iostream>
#include<WinSock2.h>
#pragma comment (lib,"ws2_32.lib")

class SockBuffer {
public:
	SOCKET sock;
	char buffer[1<<15];
	char queue[1<<15];
	int Head;
	int Tail;
	int Size;
	int Special;
	SockBuffer() ;
	int nxt(int p) ;
	void receive() ;
	std::string readString();
	void sendString(std::string&) ;
};