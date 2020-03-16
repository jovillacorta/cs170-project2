#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;


void normalize (double (&dataset)[2048][64], int numFeatures, int numInstances) {
// This function normalizes the data for us.
// returns: nothing -- our dataset is passed by reference and changed
// Normalized Data:  X = ( X-MEAN ) / STANDARD DEVIATION

	cout << "This dataset has " << numFeatures - 1 << " features (not including the class attribute), with "
			<< numInstances << " instances." << endl;

	cout << endl;
	cout << "Please wait while I normalize the data... ";

	double sum;
	double varianceNum;
	double variance;
	double mean[numFeatures];
	double std[numFeatures];
	
	// Calculating the mean for each column (feature)
	for (int j = 1; j < numFeatures; ++j){
		sum = 0;

		for (int i = 0; i < numInstances; i++){
			sum += dataset[i][j];

		}

		mean[j] = sum / numInstances;
	}

	// Calculating the standard deviation for each column (feature)
	for (int j = 1; j < numFeatures; ++j){
		varianceNum = 0;

		for (int i = 0; i < numInstances; i++){
			varianceNum += pow(dataset[i][j] - mean[j], 2);

		}

		variance = varianceNum / numInstances;
		std[j] = sqrt(variance);
	}

	// Normalizing each data point
	for (int i = 0; i < numInstances; i++){
		for (int j = 1; j < numFeatures; j++){
			dataset[i][j] = (dataset[i][j] - mean[j]) / std[j];
		}
	}

	cout << "Done!" << endl;

}

double nearestNeighbor (double dataset[2048][64], int instIndex, vector<int> featureSubset, int numInstances) {
//  This function finds the nearest neighbor of an instance given a featureSubset
//	returns: classification of nearest neighbor

	int nearestIndex = 0;
	double tempDistance = 0;
	double nearestDistance = numeric_limits<double>::max();

// loop through rows and to find row with smallest distance
// nearest distance is the MINIMUM of distances (given the feature subset)
	for (int i = 0; i < numInstances; i ++){

		tempDistance = 0;

		if (i != instIndex){

			for (int j = 0; j < featureSubset.size(); j++){
				tempDistance += pow((dataset[i][featureSubset.at(j)] - dataset[instIndex][featureSubset.at(j)]), 2);
			}

			tempDistance = sqrt(tempDistance);

			if (tempDistance < nearestDistance){
				nearestIndex = i;
				nearestDistance = tempDistance;
			}
		}
	}

	return dataset[nearestIndex][0];
}

double leaveOneOut (double dataset[2048][64], vector<int> featureSubset, int numInstances){
// This function is our leave one out algorithm 
// returns: accuracy

	int numCorrectPredict;

	for (int i = 0; i < numInstances; i++) {

		double prediction = nearestNeighbor(dataset, i, featureSubset, numInstances);

		if (dataset[i][0] == prediction){
			numCorrectPredict++;
		}
	}

	return (numCorrectPredict/numInstances) * 100;
}




int main () {

	double dataArray[2048][64];
	string fileName;
	ifstream fin;
	int numFeatures = 0;
	int numInstances = 0;
	string line;
	double word;

	cout << endl;
	cout << endl;
	cout << "Welcome to Johanna Villacorta's Feature Selection Algorithm" << endl;

	cout << endl;
	cout << "Type in the name of the file to test: ";
	cin >> fileName;

	fin.open(fileName.c_str());

	if (!fin){
		cerr << "File could not be opened." << endl;
		exit(1); 
	}



	while (getline(fin, line)){
		numFeatures = 0;
		stringstream s(line);

		while(s >> word){
			//cout << "got to instance " << numInstances << endl;
			//cout << word << endl;
			dataArray[numInstances][numFeatures] = word;
			numFeatures++;
		}

		numInstances++;
	}

	normalize(dataArray, numFeatures, numInstances);

// cout << numInstances << endl;
// cout << numFeatures <<endl;

// for (int i = 0; i < numInstances; i++){
// 	for (int j = 0; j < numFeatures; j++){
// 		cout << dataArray[i][j] << endl;
// 	}
// }



	return 0;
}