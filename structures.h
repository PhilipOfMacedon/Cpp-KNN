#ifndef STRUCTURES_H
#define STRUCTURES_H 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <tuple>

using namespace std;

/////////////////////////////////////////////////////////////////////
//////////////////////   AUXILIARY FUNCTIONS   //////////////////////

void mergeSort(tuple<int, float>* array, int length) {
	mergeSort(array, 0, length - 1);
}

void mergeSort(tuple<int, float>* array, int start, int end) {
	if ((end - start) > 1) {
		int midpoint = (end - start) / 2;
		mergeSort(array, start, midpoint - 1);
		mergeSort(array, midpoint, end);
		merge(array, start, end);
	}
}

void merge(tuple<int, float>* array, int start, int end) {
	tuple<int, float>* temp = new tuple<int, float>[end - start + 1];
	int midpoint = (end - start) / 2;
	int count1 = start;
	int count2 = midpoint;
	for (int i = 0; i <= (end - start); i++) {
		if ((count1 < midpoint and count2 > end)
		 or (count1 < midpoint and get<1>(array[count1]) < get<1>(array[count2]))) {
			temp[i] = array[count1];
			count1++;
		} else {
			temp[i] = array[count2];
			count2++;
		}
	}
	for (int i = 0; i <= (end - start); i++) {
		array[start + i] = temp[i];
	}
	delete[] temp;
}

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
		int samplesCount;
		int testedCount;
		Instance* trainingSamples;
		Instance* testingInstances;
	public:
		KNN(string fileName, float trainingSampleRatio);
		~KNN();
		int classify(Instance instance);
		int classify(Instance instance, int nnCount);
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