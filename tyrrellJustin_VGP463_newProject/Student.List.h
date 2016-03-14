// Student.List.h
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <vector>
#include <math.h>
#include <stdio.h>

using namespace std;

template <class T>
class List {
public:
	vector<T> collection;
	T* buffer;
	int length;
	List() {
		buffer = 0;
		length = 0;
	}
	T operator[](int index) {
		length = collection.size();
		// The following will trigger a runtime exception
		// if the specified index is invalid... We cannot
		// do otherwise as a value must be returned.
		return (T)this->collection[index];
	}
	int AddValue(T item) {
		collection.push_back(item);
		length = collection.size();
		return length-1;
	}
	void RemoveAt(int index) {
		collection.erase(collection.begin()+index);
		length = collection.size();
	}
	T GetValueAt(int index) {
		return (*this)[index];
	}
	T* ToBuffer() {
		if(buffer == 0) {
			buffer = new T[length];
			for(int n = 0; n < length; n++) {
				buffer[n] = GetValueAt(n);
			}
		}
		return buffer;
	}
};