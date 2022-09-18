#pragma once

#include<functional>
#include<string>
#include<map>

template<typename BaseClass>
class autoFactory {
public:
	template<typename DerivedClass>
	class registerClass {
	public:
		template<typename... Args>
		registerClass(const std::string& key, Args... args) {
			std::function<BaseClass* ()> func = [=]() {return new DerivedClass(args...); };
			autoFactory<BaseClass>::getInstance()->
				Map[key] = func;
			//Map.emplace(std::make_pair(key, [=]() {return new DerivedClass(args...); }));
		}
	};
	static BaseClass* getNormalPtr(const std::string& key) {
		std::function<BaseClass* ()> fun = autoFactory::getInstance()->find(key);
		return fun();
	}
protected:
private:
	autoFactory() {};
	autoFactory(const autoFactory&) = delete;
	autoFactory(autoFactory&&) = delete;
	autoFactory& operator=(const autoFactory&) = delete;
	static autoFactory<BaseClass>* getInstance() {
		static autoFactory<BaseClass> f;
		return &f;
	}
	std::function<BaseClass* ()> find(const std::string& key) {
		auto it = Map.find(key);
		return it->second;
	}
	std::map<std::string, std::function<BaseClass* ()>> Map;
};