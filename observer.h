#pragma once

#include<iostream>
#include<functional>

template< typename T >
class Observer {
public:
	Observer() :isValid(false) {};
	void observe(T c) {
		callback = c;
		isValid = true;
	}
	template<typename ...Args>
	void trigger(Args... args) {
		if (isValid) {
			callback(args...);
		}
	}
	void stop() {
		isValid = false;
	}
private:
	bool isValid;
	T callback;
};