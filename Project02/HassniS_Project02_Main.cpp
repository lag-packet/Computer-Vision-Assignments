// Project02 non-binary threshold - Computer Vision

#include <iostream>
#include <fstream>
//#include <string>

using namespace std;

int main(int argc, char** argv) {
    int numRows, numCols, minVal, maxVal, pixel;
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    //cout << "arg3 is " << argv[3] << endl;
    //int thrVal = atoi(argv[3]);
    int thrVal = 0;
    cout << "Please Input threshold: ";
    cin >> thrVal;   
    cout << endl;

    // Header
    inFile >> numRows >> numCols >> minVal >> maxVal;
    outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
    
    // Process pixel procedure
    int currentCol = 0;
    while (inFile >> pixel) {
        if (pixel >= thrVal) {
            outFile << pixel << " ";
        } else {
            outFile << 0 << " ";
        }
        currentCol++;
        // Row fully processed
        if (currentCol == numCols) { 
            outFile << '\n';
            currentCol = 0;
        }
    }
    inFile.close();
    outFile.close();
    return 0;

}