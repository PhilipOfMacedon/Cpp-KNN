#ifdef _WIN64
   #define CLEAR_CODE "cls"
#elif _WIN32
   #define CLEAR_CODE "cls"
#elif __linux
    #define CLEAR_CODE "clear"
#elif __unix
    #define CLEAR_CODE "clear"
#elif __posix
    #define CLEAR_CODE "clear"
#endif

#include "structures.h"

int main(int argc, char const **argv) {
	system(CLEAR_CODE);
	if (argc == 3) {
		string instanceFileName = string(argv[1]);
		string configFileName = string(argv[2]);
		ifstream ifn(instanceFileName.c_str());
		ifstream cfn(configFileName.c_str());
		if (ifn.good() and cfn.good()) {
			ifn.close();
			cfn.close();
			Configuration cfg(configFileName);
			cout << "Reading file... ";
			KNN classifier(instanceFileName, cfg.instanceCount, cfg.attributeCount, 
				cfg.trainingRatio, cfg.classNames, cfg.hasNumericClassNames);
			cout << "Done!" << endl;
			cout << "Classifying tests.";
			classifier.classifyAllTests(cfg.nnCount);
			cout << " Done!" << endl;
			classifier.printConfusionMatrix();
		} else {
			cout << "Problem reading files." << endl;
		}
	} else {
		cout << "USAGE: " << argv[0] << " [INSTANCE FILE NAME] [CONFIGURATION FILE NAME]" << endl;
	}	

	cout << endl;

	return 0;
}