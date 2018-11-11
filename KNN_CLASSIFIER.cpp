#include "structures.h"

/*
string fileName = "instances/spambase.data";
int instCount = 4601;
int attribCount = 57;
float ratio = 0.6;
bool isNumeric = true;
int classCount = 2;
*/
string fileName = "instances/iris.data";
int instCount = 150;
int attribCount = 4;
float ratio = 0.9;
bool isNumeric = false;
int classCount = 3;

vector<string> labels;

int main(int argc, char const **argv) {
	if (argc > 1) {
		fileName = string(argv[1]);
		cout << "# OF INSTANCES: ";
		cin >> instCount;
		cout << "# OF ATTRIBUTES: ";
		cin >> attribCount;
		cout << "TRAINING SAMPLE RATIO: ";
		cin >> ratio;
		cout << "HAS NUMERIC NAMES ON FILE? (0 = NO; 1 = YES): ";
		int numeric;
		cin >> numeric;
		if (numeric == 0) {
			isNumeric = false;
		} else {
			isNumeric = true;
		}
		cout << "# OF CLASSES: ";
		cin >> classCount;
		string label;
		for (int i = 0; i < classCount; i++) {
			cout << "NAME OF CLASS #" << i << ": ";
			cin >> label;
			labels.push_back(label);
		}
	} else {
		/*
		labels.push_back("NOT-SPAM");
		labels.push_back("SPAM");
		*/
		labels.push_back("Iris-setosa");
		labels.push_back("Iris-versicolor");
		labels.push_back("Iris-virginica");
		
	}
	ifstream input(fileName.c_str());
	if (input.good()) {
		input.close();
		KNN classifier(fileName, instCount, attribCount, ratio, labels, isNumeric);
		cout << "Classifying tests.";
		classifier.classifyAllTests();
		cout << " Done!" << endl;
		classifier.printConfusionMatrix();
	} else {
		cout << "Problem reading file." << endl;
	}

	cout << endl;

	return 0;
}