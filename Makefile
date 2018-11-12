# Makefile - KNN_CLASSIFIER #

all: knn_classifier

knn_classifier: knn_classifier.cpp structures.h
	g++ -std=c++11 knn_classifier.cpp -o KNN_CLASSIFIER

clean:
	rm *.o knn_classifier