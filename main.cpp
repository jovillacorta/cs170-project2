#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

void printSubset (vector<int> subset){
// helper function to print our feature subsets
// returns: void

	cout << "{";
	for (int i =0; i < subset.size(); i++){
		cout << subset.at(i);
		if (i != subset.size() - 1){
			cout << ", ";
		}
	}
	cout << "}";
}

void normalize (double (&dataset)[2048][64], int numInstances, int numFeatures) {
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

	int numCorrectPredict = 0;

	for (int i = 0; i < numInstances; i++) {

		double prediction = nearestNeighbor(dataset, i, featureSubset, numInstances);

		if (dataset[i][0] == prediction){
			numCorrectPredict++;
		}
	}

//	cout << numCorrectPredict<< endl;
//	cout << (double(numCorrectPredict)/numInstances * 100) << endl;
	return (double(numCorrectPredict)/numInstances * 100);
}

void forwards (double dataset[2048][64], int numInstances, int numFeatures){
// This function performs a forwardSelection greedy search
// returns: nothing (prints subset with greatest accuracy)
	cout << endl;
	cout << "Beginning search." << endl;

	// hold our selected feature subset and solution subset
	vector<int> selectedSubset;
	vector<int> solution;

	double tempAccuracy;
	double maxAccuracy;
	double maxMaxAccuracy = 0;
	bool foundSolutionSubset;

	int maxIndex;

	for (int i = 1; i < numFeatures; i++){
		foundSolutionSubset = false;
		maxAccuracy = 0;
		maxIndex = 0;
		cout << endl;
		for (int j = 1; j < numFeatures; j++){
			//cout << endl;
			if (!(find(selectedSubset.begin(), selectedSubset.end(), j) != selectedSubset.end())){
				vector<int> temp = selectedSubset;
				temp.push_back(j);

				tempAccuracy = leaveOneOut(dataset, temp, numInstances);

				// print statement
				cout << "		Using feature(s) ";
				printSubset(temp);
				cout << " accuracy is " << tempAccuracy << "%" << endl;

				// max accuracy amongst particular set of subsets
				if (tempAccuracy > maxAccuracy){
					maxAccuracy = tempAccuracy;
					maxIndex = j;

					// max accuracy of ALL subsets
					if (tempAccuracy > maxMaxAccuracy){
						maxMaxAccuracy = tempAccuracy;
						foundSolutionSubset = true;
					}
				}

			}

		}
		cout << endl;
		selectedSubset.push_back(maxIndex);

		if (foundSolutionSubset){
			solution.push_back(maxIndex);
		}
		else{
			if (selectedSubset.size() != numFeatures - 1){
			cout << "(Warning: Accuracy has decreased! Continuing search in case of local maxima)" << endl;
			}
		}


		if (selectedSubset.size() != numFeatures - 1){
			cout << "Feature set ";
			printSubset(selectedSubset);
			cout << " was best, accuracy is, " << maxAccuracy << "%" << endl;
		}
	}

	cout << endl;
	cout << "Finished search!! The best feature subset is ";
	printSubset(solution);
	cout << ", which has an accuracy of " << maxMaxAccuracy << "%" << endl;
	cout << endl;
	//return solution;
}

void backwards (double dataset[2048][64], int numInstances, int numFeatures){
	
}

int main () {

	double dataArray[2048][64];
	string fileName;
	ifstream fin;
	int numFeatures = 0;
	int numInstances = 0;
	string line;
	double word;
	int input;

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
	normalize(dataArray, numInstances, numFeatures);

error1:
		cout << endl;
		cout << "Type the number of the algorithm you want to run" << endl;
		cout << endl;
		cout << "1) Forward Selection" << endl;
		cout << "2) Backward Selection" << endl;
		cout << endl;

		cin >> input;

		switch(input)
		{
			case 1:	
				forwards(dataArray, numInstances, numFeatures);
				break;
			case 2:
				break;
			default:
				cout << "Invalid Entry." << endl;
				goto error1;
		}

// cout << numInstances << endl;
// cout << numFeatures <<endl;

// for (int i = 0; i < numInstances; i++){
// 	for (int j = 0; j < numFeatures; j++){
// 		cout << dataArray[i][j] << endl;
// 	}
// }



	return 0;
}