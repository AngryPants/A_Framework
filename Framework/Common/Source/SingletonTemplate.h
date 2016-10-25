#ifndef SINGLETON_H
#define SINGLETON_H

template <class Type>
class Singleton {

private:
	static Type* instance;

protected:
	//Constructor(s) & Destructor
	Singleton() {}
	virtual ~Singleton() {}

public:
	static Type& GetInstance() {
		if (instance == nullptr) {
			instance = new Type();
		}

		return *instance;
	}

	static bool Destroy() {
		if (instance == nullptr) {
			return false;
		}
		delete instance;
		instance = nullptr;
		return true;
	}

};

template <class Type>
Type* Singleton<Type>::instance = nullptr;

#endif