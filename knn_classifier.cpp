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
			cerr << "Reading file... ";
			KNN classifier(instanceFileName, cfg.instanceCount, cfg.attributeCount, 
				cfg.trainingRatio, cfg.classNames, cfg.hasNumericClassNames);
			cerr << "Done!" << endl;
			cerr << "Classifying tests.";
			classifier.classifyAllTests(cfg.nnCount);
			cerr << " Done!" << endl;
			classifier.printConfusionMatrix();
		} else {
			cerr << "Problem reading files." << endl;
		}
	} else {
		cerr << "USAGE: " << argv[0] << " [INSTANCE FILE NAME] [CONFIGURATION FILE NAME]" << endl;
	}	

	cerr << endl;

	return 0;
}