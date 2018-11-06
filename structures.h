#ifndef STRUCTURES_H
#define STRUCTURES_H 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

class Instance {
	private:
		int identifier;
		int attribNumber;
		float* attribs;
		int instanceClass;
		bool forTraining;
	public:
		Instance(int id, int attNum);
		~Instance();
		string loadAttribs(string attribLine);
		float* getAttribs();
		int getAttNumber();
		int getClass();
		int getId();
		int setClass(int instClass);
};

class KNN {
	private:
		
}

Instance::Instance(int id, int attNum) {
	identifier = id;
	attribNumber = attNum;
	instanceClass = -1;
	attribs = new float[attribNumber];
}

Instance::~Instance() {
	delete[] attribs;
}

string Instance::loadAttribs(string attribLine) {
	stringstream ss(attribLine);
	for (int i = 0; i < attribNumber; i++) {
		if (ss.good()) {
			ss >> attribs[i];
			if (ss.peek() == ',') {
				ss.ignore();
			}
		}
	}
	string literalClass;
	ss >> literalClass;
	return literalClass;
}

float* Instance::getAttribs() {
	return attribs;
}

int Instance::getAttNumber() {
	return attribNumber;
}

int Instance::getClass() {
	return instanceClass;
}

int Instance::getId() {
	return identifier;
}

int Instance::setClass(int instClass) {
	instanceClass = instClass;
}

#endif