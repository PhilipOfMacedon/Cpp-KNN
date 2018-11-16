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
#include <ctime>
#include <vector>

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

string loadHTMLHeader() {
	ifstream input("res/header.template");
	string header = "";
	if (!input.good()) {
		cerr << "HTML resource not found, skipping file generation." << endl;
		return "";
	}
	while (input.good()) {
		string line;
		input >> line;
		header += line + "\n";
	}
	return header;
}

/////////////////////////////////////////////////////////////////////
//////////////////////   CLASSES DEFINITIONS   //////////////////////

class Statistics {
	public:
		int successes;
		int failures;
		float accuracy;
		float* recall;
		float* precision;
		float* f1Score;

		Statistics(int** confusionMatrix, int classCount);
		Statistics() {}
		~Statistics();
};

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
		void generateCMTable(ofstream& output);
		void generateICTable(ofstream& output);
		void generateStatsReport(ofstream& output);
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
		void generateHTML();
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

////////////////////// CLASS Statistics

Statistics::Statistics(int** confusionMatrix, int classCount) {
	float num = 0;
	float den = 0;
	successes = 0;
	failures = 0;
	recall = new float[classCount];
	precision = new float[classCount];
	f1Score = new float[classCount];

	for (int i = 0; i < classCount; i++) {
		recall[i] = 0;
		precision[i] = 0;
	}

	for (int i = 0; i < classCount; i++) {
		num += confusionMatrix[i][i];
		for (int j = 0; j < classCount; j++) {
			if (i == j) {
				successes += confusionMatrix[i][j];
			} else {
				failures += confusionMatrix[i][j];
			}
			den += confusionMatrix[i][j];
			recall[i] += confusionMatrix[i][j];
			precision[j] += confusionMatrix[i][j];
		}
	}

	accuracy = 100 * num / den;

	for (int i = 0; i < classCount; i++) {
		float auxR = (recall[i] != 0) ? (confusionMatrix[i][i] / recall[i]) : (0); 
		float auxP = (precision[i] != 0) ? (confusionMatrix[i][i] / precision[i]) : (0);
		recall[i] = auxR * 100;
		precision[i] = auxP * 100;
		f1Score[i] = (auxP + auxR != 0) ? (200 * (auxR * auxP) / (auxR + auxP)) : (0);
	}
}

Statistics::~Statistics() {
	delete[] recall;
	delete[] precision;
	delete[] f1Score;
}

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
	for (int trainingCount = instCount * ratio; trainingCount >= 0; trainingCount--) {
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
	if (nnCount > trainingSamples.size()) {
		nnCount = trainingSamples.size();
	}
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
	int intervalCount = 1;
	int percentageCount = 10;
	for (int i = 0; i < testingInstances.size(); i++) {
		classify(testingInstances[i], nnCount);
		float num = i;
		float den = testingInstances.size();
		if (num / den > (0.1 * (float)intervalCount) / 3.0) {
			if (intervalCount % 3 == 0) {
				cerr << percentageCount << "%";
				percentageCount += 10;
			} else {
				cerr << ".";
			}
			intervalCount++;
		}
	}
	cerr << "100% ";
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

void KNN::generateCMTable(ofstream& output) {
	int order = orderedLabels.size();
	output << "<div class=\"col-md-4 col-md-offset-4\">\n";
	output << "<h3>Confusion Matrix:</h3>\n";
	output << "<table class=\"table table-bordered table-hover\">\n";
	output << "<tbody>\n";
	output << "<tr>\n<th></th>";
	for (int i = 0; i < order; i++) {
		output << "<th>" << orderedLabels[i] << "</th>\n";
	}
	output << "</tr>\n";
	for (int i = 0; i < order; i++) {
		output << "<tr>\n<th>" << orderedLabels[i] << "</th>\n";
		for (int j = 0; j < order; j++) {
			output << "<td";
			if (i == j) {
				output << " class=\"success\">";
			} else {
				output << " class=\"danger\">";
			}
			output << confusionMatrix[i][j] << "</td>\n";
		}
		output << "</tr>\n";
	}
	output << "</tbody>\n</table>\n</div>\n<br>\n";
}

void KNN::generateICTable(ofstream& output) {
	output << "<div class=\"col-md-12\" style=\"padding-left:60px;padding-right:60px;\">\n";
	output << "<h3>Classified Instances:</h3>\n";
	output << "<table class=\"table table-bordered table-hover\"\n>";
	output << "<tbody>\n";
	output << "<tr>\n<th>Instance #</th\n>";
	output << "<th colspan=\"" << attribCount << "\">Parameters</th>\n";
	output << "<th>Actual Class</th>\n";
	output << "<th>Classified As</th>\n";
	output << "</tr>\n";
	for (int i = 0; i < testingInstances.size(); i++) {
		string color;
		if (testingInstances[i].getInstanceClass() == testingInstances[i].getClassifiedClass()) {
			color = "success";
		} else {
			color = "danger";
		}
		output << "<tr class=\"" << color << "\">\n";
		output << "<th>" << (i + 1) << "</th>";
		vector<float> attribs = testingInstances[i].getAttribs();
		for (int j = 0; j < attribs.size(); j++) {
			output << "<td>" << attribs[j] << "</td>\n";
		}
		string storedName = testingInstances[i].getInstanceClass();
		int id = cfMatrixLabels[storedName];
		output << "<td>" << orderedLabels[id] << "</td>\n";
		storedName = testingInstances[i].getClassifiedClass();
		id = cfMatrixLabels[storedName];
		output << "<td>" << orderedLabels[id] << "</td>\n";
		output << "</tr>\n";
	}
	output << "</tbody>\n</table>\n</div>\n<br>\n";
}

void KNN::generateStatsReport(ofstream& output) {
	Statistics stats(confusionMatrix, classCount);
	output << "<div class=\"col-sm-2 col-sm-offset-5\">\n";
	output << "<h3>Statistics:</h3>\n";
	output << "<table class=\"table table-hover\">\n";
	output << "<tbody>\n<tr class=\"info\">\n";
	output << "<th>Correctly classified:</th>\n";
	output << "<td>" << stats.successes << "</td>\n</tr>\n";
	output << "<tr class=\"info\">\n<th>Incorrectly classified:</th>";
    output << "<td>" << stats.failures << "</td>\n</tr>";
    output << "<tr class=\"info\">\n<th>Total classified:</th>";
    output << "<td>" << (stats.successes + stats.failures) << "</td>\n</tr>";
    for (int i = 0; i < classCount; i++) {
    	string tableLine;
    	if (i % 2 == 0) {
    		tableLine = "<tr class=\"active\">";
    	} else {
    		tableLine = "<tr>";
    	}
    	output << tableLine;
    	output << "<th>" << orderedLabels[i] << " Recall:</th>\n";
    	output << "<td>" << stats.recall[i] << "%</td>\n</tr>\n";
    	output << tableLine;
    	output << "<th>" << orderedLabels[i] << " Precision:</th>\n";
    	output << "<td>" << stats.precision[i] << "%</td>\n</tr>\n";
    	output << tableLine;
    	output << "<th>" << orderedLabels[i] << " F1-score:</th>\n";
    	output << "<td>" << stats.f1Score[i] << "%</td>\n</tr>\n";
    }
    output << "<tr class=\"info\">\n<th>Accuracy:</th>\n";
    output << "<td>" << stats.accuracy << "%</td>\n</tr>\n";
    output << "</tbody>\n</table>\n</div>\n<br>\n";
}

void KNN::generateHTML() {
	ofstream output("html/report.html");
	string header = loadHTMLHeader();
	if (header != "") {
		output << header;
		generateICTable(output);
		generateCMTable(output);
		generateStatsReport(output);
		output << "</body>\n</html>";
	}
	cerr << endl << "Report generated!" << endl;
	output.close();
	system("firefox html/report.html &");
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