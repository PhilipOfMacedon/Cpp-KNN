#ifndef STRUCTURES_H
#define STRUCTURES_H 1
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <cmath>
#include <map>

using namespace std;

/////////////////////////////////////////////////////////////////////
//////////////////////   AUXILIARY FUNCTIONS   //////////////////////

void merge(tuple<int, float>* array, int start, int end) {
	tuple<int, float>* temp = new tuple<int, float>[end - start + 1];
	int midpoint = start + (end - start) / 2;
	int count1 = start;
	int count2 = midpoint + 1;
	for (int i = 0; i <= (end - start); i++) {
		if (count1 <= midpoint) {
			if (count2 > end or get<1>(array[count1]) <= get<1>(array[count2])) {
				temp[i] = array[count1];
				count1++;
			} else {
				temp[i] = array[count2];
				count2++;
			}
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

void mergeSort(tuple<int, float>* array, int start, int end) {
	if ((end - start) > 0) {
		int midpoint = start + (end - start) / 2;
		mergeSort(array, start, midpoint);
		mergeSort(array, midpoint + 1, end);
		merge(array, start, end);
	}
}

void mergeSort(tuple<int, float>* array, int length) {
	mergeSort(array, 0, length - 1);
}


/////////////////////////////////////////////////////////////////////
//////////////////////   CLASSES DEFINITIONS   //////////////////////

class Instance {
	private:
		vector<float> attribs;
		string instanceClass;
		string classifiedClass;
	public:
		Instance(string attLine, int attNum);
		Instance(const Instance& another);
		~Instance() {}
		void loadAttribs(string attribLine, int attribCount);
		void setClassifiedClass(string newClass);
		vector<float> getAttribs();
		string getInstanceClass();
		string getClassifiedClass();
		string getClassificationStatus();
};

class KNN {
	private:
		int classCount;
		int attribCount;
		float trainingSampleRatio;
		vector<Instance> trainingSamples;
		vector<Instance> testingInstances;
		vector<string> orderedLabels;
		bool tested;
		bool hasNumericClassNames;
		map<string, int> cfMatrixLabels;
		int** confusionMatrix;

		void loadFile(string fileName, int instCount, float ratio);
		void initLabelMap();
		void generateConfusionMatrix();
		float calculateEuclideanDistance(vector<float> u, vector<float> v);
		map<string, int> countClassOccurrences(tuple<int, float>* distances, int nnCount);
		string getGreatestOccurrence(map<string, int> occurrences, tuple<int, float>* distances, int nnCount);
	public:
		KNN(string fileName, int instCount, int attCount, float ratio, vector<string> labels, bool isNumeric);
		~KNN();
		string classify(Instance& instance);
		string classify(Instance& instance, int nnCount);
		void classifyAllTests();
		void classifyAllTests(int nnCount);
		int getClassCount();
		int** getConfusionMatrix();
		void printConfusionMatrix();
};

class Configuration {
	private:
		void loadFile(string fileName);
	public:
		int instanceCount;
		int attributeCount;
		float trainingRatio;
		bool hasNumericClassNames;
		int nnCount;
		int classCount;
		vector<string> classNames;

		Configuration(string fileName);
};

/////////////////////////////////////////////////////////////////////
////////////////////// CLASSES IMPLEMENTATIONS //////////////////////

////////////////////// CLASS Instance

Instance::Instance(string attLine, int attNum) {
	instanceClass = "UNINFORMED";
	classifiedClass = "UNCLASSIFIED";
	loadAttribs(attLine, attNum);
}

Instance::Instance(const Instance& another) {
	instanceClass = another.instanceClass;
	classifiedClass = another.classifiedClass;
	attribs = another.attribs;
}

void Instance::loadAttribs(string attribLine, int attribCount) {
	stringstream ss(attribLine);
	for (int i = 0; i < attribCount; i++) {
		if (ss.good()) {
			float value;
			ss >> value;
			attribs.push_back(value);
			if (ss.peek() == ',') {
				ss.ignore();
			}
		}
	} 
	ss >> instanceClass;
}

void Instance::setClassifiedClass(string newClass) {
	classifiedClass = newClass;
}

vector<float> Instance::getAttribs() {
	return attribs;
}

string Instance::getInstanceClass() {
	return instanceClass;
}

string Instance::getClassifiedClass() {
	return classifiedClass;
}

string Instance::getClassificationStatus() {
	string response = "";
	if (instanceClass == "UNINFORMED") {
		response += "UNINFORMED  AND ";
		if (classifiedClass == "UNCLASSIFIED") {
			response += "UNCLASSIFIED";
		} else {
			response += "CLASSIFIED";
		}
	} else {
		if (classifiedClass == "UNCLASSIFIED") {
			response = "UNCLASSIFIED";
		} else {
			if (instanceClass == classifiedClass) {
				response += "CORRECTLY ";
			} else {
				response += "INCORRECTLY ";
			}
			response += "CLASSIFIED";
		}
	}
	return response;
}

////////////////////// CLASS KNN


KNN::KNN(string fileName, int instCount, int attCount, float ratio, vector<string> labels, bool isNumeric) {
	hasNumericClassNames = isNumeric;
	attribCount = attCount;
	orderedLabels = labels;
	classCount = labels.size();
	loadFile(fileName, instCount, ratio);
	initLabelMap();
	confusionMatrix = NULL;
}


KNN::~KNN() {
	for (int i = 0; i < classCount; i++) {
		delete[] confusionMatrix[i];
	}
	delete[] confusionMatrix;
}

void KNN::loadFile(string fileName, int instCount, float ratio) {
	vector<Instance> instances;
	ifstream input(fileName.c_str());
	while (input.good()) {
		string line;
		getline(input, line);
		instances.push_back(Instance(line, attribCount));
	}
	input.close();
	srand(time(NULL));
	for (int trainingCount = instCount * ratio; trainingCount > 0; trainingCount--) {
		int pos = rand() % instances.size();
		trainingSamples.push_back(instances[pos]);
		instances.erase(instances.begin() + pos);
	}
	for (int i = 0; i < instances.size(); i++) {
		testingInstances.push_back(instances[i]);
	}
}

void KNN::initLabelMap() {
	if (hasNumericClassNames) {
		for (int i = 0; i < orderedLabels.size(); i++) {
			stringstream ss;
			ss << i;
			cfMatrixLabels[ss.str()] = i;
		}
	} else {
		for (int i = 0; i < orderedLabels.size(); i++) {
			cfMatrixLabels[orderedLabels[i]] = i;
		}
	}
}

void KNN::generateConfusionMatrix() {
	if (confusionMatrix == NULL) {
		confusionMatrix = new int*[classCount];
		for (int i = 0; i < classCount; i++) {
			confusionMatrix[i] = new int[classCount];
			for (int j = 0; j < classCount; j++) {
				confusionMatrix[i][j] = 0;
			}
		}
	} else {
		for (int i = 0; i < classCount; i++) {
			for (int j = 0; j < classCount; j++) {
				confusionMatrix[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < testingInstances.size(); i++) {
		int line = cfMatrixLabels[testingInstances[i].getInstanceClass()];
		int column = cfMatrixLabels[testingInstances[i].getClassifiedClass()];
		confusionMatrix[line][column] = confusionMatrix[line][column] + 1;
	}
}

float KNN::calculateEuclideanDistance(vector<float> u, vector<float> v) {
	float distance = 0;
	for (int i = 0; i < attribCount; i++) {
		distance += (pow((u[i] - v[i]), 2));
	}
	return sqrt(distance);
}

map<string, int> KNN::countClassOccurrences(tuple<int, float>* distances, int nnCount) {
	map<string, int> occurrences;
	for (int i = 0; i < nnCount; i++) {
		int id = get<0>(distances[i]);
		string instanceClass = trainingSamples[id].getInstanceClass(); 
		occurrences[instanceClass]++;
	}
	return occurrences;
}

string KNN::getGreatestOccurrence(map<string, int> occurrences, tuple<int, float>* distances, int nnCount) {
	string greatestClass;
	int greatest = -1;
	for (auto const& entry : occurrences) {
	    if (entry.second > greatest) {
	    	greatestClass = entry.first;
	    	greatest = entry.second;
	    } else if (entry.second == greatest) {
	    	greatestClass = "REPEATED";
	    }
	}
	if (greatestClass == "REPEATED") {
		int id = get<0>(distances[nnCount - 1]);
		string instanceClass = trainingSamples[id].getInstanceClass(); 
		occurrences[instanceClass]--;
		return getGreatestOccurrence(occurrences, distances, nnCount - 1);
	}
	return greatestClass;
}

string KNN::classify(Instance& instance) {
	int nnCount = sqrt(trainingSamples.size());
	if (classCount == 2 and nnCount % 2 == 0) {
		nnCount++;
	}
	return classify(instance, nnCount);
}

string KNN::classify(Instance& instance, int nnCount) {
	tuple<int, float>* distances = new tuple<int, float>[trainingSamples.size()];
	for (int i = 0; i < trainingSamples.size(); i++) {
		distances[i] = make_tuple(i, 
			calculateEuclideanDistance(instance.getAttribs(), trainingSamples[i].getAttribs()));
	}
	mergeSort(distances, trainingSamples.size());
	map<string, int> occurrences = countClassOccurrences(distances, nnCount);
	string determinedClass = getGreatestOccurrence(occurrences, distances, nnCount);
	instance.setClassifiedClass(determinedClass);
	return determinedClass;
}

void KNN::classifyAllTests() {
	int nnCount = sqrt(trainingSamples.size());
	if (classCount == 2 and nnCount % 2 == 0) {
		nnCount++;
	}
	classifyAllTests(nnCount);
}

void KNN::classifyAllTests(int nnCount) {
	int updateInterval = testingInstances.size()/30;
	int intervalCount = 1;
	for (int i = 0; i < testingInstances.size(); i++) {
		classify(testingInstances[i], nnCount);
		if (i > updateInterval * intervalCount) {
			if (intervalCount % 3 != 0) {
				cout << ".";
			} else {
				cout << intervalCount * 10 / 3 << "%";
			}
			intervalCount++;
		}
	}
	generateConfusionMatrix();
}

int KNN::getClassCount() {
	return classCount;
}

int** KNN::getConfusionMatrix() {
	return confusionMatrix;
}

void KNN::printConfusionMatrix() {
	int greatestLength = 0;
	for (int i = 0; i < classCount; i++) {
		if (orderedLabels[i].length() > greatestLength) {
			greatestLength = orderedLabels[i].length();
		}
	}
	int greatestTabCount = floor(greatestLength/8.0) + 1;
	vector<string> tabs;
	for (int i = 0; i < classCount; i++) {
		int tabCount = greatestTabCount - floor(orderedLabels[i].length()/8);
		string tab = "";
		for (int j = 0; j < tabCount; j++) {
			tab += "\t";
		}
		tabs.push_back(tab);
	}
	string greatestTab = "";
	for (int j = 0; j < greatestTabCount; j++) {
		greatestTab += "\t";
	}
	cout << greatestTab;
	for (int i = 0; i < classCount; i++) {
		cout << orderedLabels[i] << tabs[i];
	}
	cout << endl;
	for (int i = 0; i < classCount; i++) {
		cout << orderedLabels[i] << tabs[i];
		for (int j = 0; j < classCount; j++) {
			cout << confusionMatrix[i][j] << greatestTab;
		}
		cout << endl;
	}
}

////////////////////// CLASS Configuration

Configuration::Configuration(string fileName) {
	loadFile(fileName);
}

void Configuration::loadFile(string fileName) {
	instanceCount = 0;
	attributeCount = 0;
	trainingRatio = 0.6;
	hasNumericClassNames = false;
	nnCount = 0;

	ifstream input(fileName.c_str());
	string line;
	while (input.good()) {
		getline(input, line);
		if (line[0] == '#') continue;
		stringstream ss(line);
		string statement;
		ss >> statement;
		if (statement.find("instCount") < 20) {
			ss >> instanceCount;
			nnCount = sqrt(instanceCount);
		} else if (statement.find("attribCount") < 20) {
			ss >> attributeCount;
		} else if (statement.find("ratio") < 20) {
			ss >> trainingRatio;
		} else if (statement.find("isNumeric") < 20) {
			ss >> statement;
			if (statement == "true") {
				hasNumericClassNames = true;
			} else {
				hasNumericClassNames = false;
			}
		} else if (statement.find("kValue") < 20) {
			ss >> statement;
			if (statement.find("set") >= 0) {
				ss >> nnCount;
			}
		} else if (statement.find("classCount") < 20) {
			ss >> classCount;
		} else if (statement.find("classNames") < 20) {
			while (ss.good()) {
				ss >> statement;
				classNames.push_back(statement);
			}
		}
	}
	input.close();
}



/////////////////////////////////////////////////////////////////////
//////////////////////       END OF FILE       //////////////////////

#endif