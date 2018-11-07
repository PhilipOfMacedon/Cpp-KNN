#ifndef STRUCTURES_H
#define STRUCTURES_H 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

/////////////////////////////////////////////////////////////////////
//////////////////////   AUXILIARY FUNCTIONS   //////////////////////

void mergeSort()

/////////////////////////////////////////////////////////////////////
//////////////////////   CLASSES DEFINITIONS   //////////////////////

class Instance {
	private:
		int identifier;
		int attribCount;
		float* attribs;
		int instanceClass;
	public:
		Instance(int id, int attNum);
		Instance(const Instance& another);
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
		int instanceCount;
		Instance* instances;
	public:
		KNN(int instCount)
};

/////////////////////////////////////////////////////////////////////
////////////////////// CLASSES IMPLEMENTATIONS //////////////////////

Instance::Instance(int id, int attNum) {
	identifier = id;
	attribCount = attNum;
	instanceClass = -1;
	attribs = new float[attribCount];
}

Instance::Instance(const Instance& another) {
	identifier = another.identifier;
	attribCount = another.attribCount;
	instanceClass = another.instanceClass;
	attribs = new float[attribCount];
	copy(begin(another.attribs), end(another.attribs), begin(attribs));
}

Instance::~Instance() {
	delete[] attribs;
}

string Instance::loadAttribs(string attribLine) {
	stringstream ss(attribLine);
	for (int i = 0; i < attribCount; i++) {
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
	return attribCount;
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

/////////////////////////////////////////////////////////////////////
//////////////////////       END OF FILE       //////////////////////

#endif