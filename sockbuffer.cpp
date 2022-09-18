
#include"sockbuffer.h"

SockBuffer::SockBuffer() {
	Head = 1; Tail = 0;
	Size = 0; Special = 0;
}

int SockBuffer::nxt(int p) {
	return (p + 1) % (1 << 15);
}

void SockBuffer::receive() {
	int num = recv(sock, buffer, 1 << 15, 0);
	for (int i = 0; i < num; i++) {
		queue[Tail = nxt(Tail)] = buffer[i];
		Size++;
		if (buffer[i] == '$')Special++;
		if (Size > (1 << 15)) {
			std::cout << "queue is full\n";
			exit(0);
		}
	}
}

std::string SockBuffer::readString() {
	while (Special == 0)receive();
	std::string str;
	while (Size > 0) {
		char c = queue[Head];
		Head = nxt(Head);
		Size--;
		if (c == '$') {
			Special--;
			break;
		}str.push_back(c);
	}
	return str;
}

void SockBuffer::sendString(std::string& str) {
	str.push_back('$');
	send(sock,str.c_str(),str.length(),0);
	std::cout<<"send_str="<<str<<"\n";
	str.pop_back();
}