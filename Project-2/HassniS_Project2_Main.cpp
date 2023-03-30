#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

class Enhancement { 
    public:
        int numRows, numCols, minVal, maxVal,
            maskRows, maskCols, maskMin, maskMax, thrVal;
        int** mirrorFramedAry;
        int** avgAry;
        int** medianAry;
        int** gaussAry;
        int** thrAry; 
        int* neighborAry;
        int* maskAry;
        int maskWeight;

        Enhancement(int numRows, int numCols, int minVal, int maxVal,
                        int maskRows, int maskCols, int maskMin, int maskMax) {
            cout << "Creating Enhancement object" << endl;
            this->numRows = numRows;
            this->numCols = numCols;
            this->minVal = minVal;
            this->maxVal = maxVal;
            this->maskRows = maskRows;
            this->maskCols = maskCols;
            this->maskMin = maskMin;
            this->maskMax = maskMax;
            maskWeight = 0;
            mirrorFramedAry = new int*[numRows + 4];
            avgAry = new int*[numRows + 4];
            medianAry = new int*[numRows + 4];
            gaussAry = new int*[numRows + 4];
            thrAry = new int*[numRows + 4];
            for (int i = 0; i < numRows + 4; i++) {
                mirrorFramedAry[i] = new int[numCols + 4];
                avgAry[i] = new int[numCols + 4];
                medianAry[i] = new int[numCols + 4];
                gaussAry[i] = new int[numCols + 4];
                thrAry[i] = new int[numCols + 4];       
            }
            for (int i = 0; i < numRows + 4; i++) {
                for (int j = 0; j < numCols + 4; j++) {
                    mirrorFramedAry[i][j] = 0;
                    avgAry[i][j] = 0;
                    medianAry[i][j] = 0;
                    gaussAry[i][j] = 0;
                    thrAry[i][j] = 0;
                }
            }
            neighborAry = new int[25];
            maskAry = new int[25];
        }
        
        ~Enhancement() {
            for (int i = 0; i < numRows + 4; i++) {
                delete[] mirrorFramedAry[i];
                delete[] avgAry[i];
                delete[] medianAry[i];
                delete[] gaussAry[i];
                delete[] thrAry[i];
            }
            delete[] neighborAry;
            delete[] maskAry;
            delete[] mirrorFramedAry;
            delete[] avgAry;
            delete[] medianAry;
            delete[] gaussAry;
            delete[] thrAry;
    }

        void binaryThreshold(int** inAry) {
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (inAry[i][j] >= thrVal) {
                        thrAry[i][j] = 1;
                    } else {
                        thrAry[i][j] = 0;
                    }
                }
            }
        }

        void loadImage(ifstream *inFile) {
            for (int i = 2; i < numRows + 2; i++) {
                for (int j = 2; j < numCols + 2; j++) {
                    *inFile >> mirrorFramedAry[i][j];
                    //cout << "mirrorFramedArr i,j:" << i << " " << j << endl;
                }
            }
    
            //cout << "numRows + 2 = " << numRows + 2 << " numCols + 2 = " << numCols + 2 << endl;
        }

        void mirrorFraming() {
            //mirror pad the corners
            cout << "mirror padding" << endl;
            int topLeftCorner = mirrorFramedAry[2][2];
            int topRightCorner = mirrorFramedAry[2][numCols + 1];
            int bottomLeftCorner = mirrorFramedAry[numRows + 1][2];
            int bottomRightCorner = mirrorFramedAry[numRows + 1][numCols + 1];
            
            // topleft
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    mirrorFramedAry[i][j] = topLeftCorner;
                }
            }

            // topright
            for (int i = 0; i < 3; i++) {
                for (int j = numCols + 3; j > numCols; j--) {
                    mirrorFramedAry[i][j] = topRightCorner;
                }
            }

            // bottom left
            for (int i = numRows + 3; i > numRows; i--) {
                for (int j = 0; j < 3; j++) {
                    mirrorFramedAry[i][j] = bottomLeftCorner;
                }
            }
            cout << "bot left done" << endl;

            // bottom right
            for (int i = numRows + 3; i > numRows; i--) {
                for (int j = numCols + 3; j > numCols; j--) {
                    mirrorFramedAry[i][j] = bottomRightCorner;
                }
            } // mirror pad corners end

            // mirror pad columns
            for (int i = 2; i < numRows + 2; i++) {
                mirrorFramedAry[i][0] = mirrorFramedAry[i][2];
                mirrorFramedAry[i][1] = mirrorFramedAry[i][2];
                mirrorFramedAry[i][numCols + 2] = mirrorFramedAry[i][numCols + 1];
                mirrorFramedAry[i][numCols + 3] = mirrorFramedAry[i][numCols + 1];
            }

            // mirror pad rows
            for (int j = 0; j < numCols + 4; j++) {
                mirrorFramedAry[0][j] = mirrorFramedAry[2][j];
                mirrorFramedAry[1][j] = mirrorFramedAry[2][j];
                mirrorFramedAry[numRows + 2][j] = mirrorFramedAry[numRows + 1][j];
                mirrorFramedAry[numRows + 3][j] = mirrorFramedAry[numRows + 1][j];
            }

            // Print the padded matrix
            for (int i = 0; i < numRows + 4; i++) {
                for (int j = 0; j < numCols + 4; j++) {
                    cout << mirrorFramedAry[i][j] << " ";
                }
                cout << endl;
            
            }
        }


        void loadMaskAry(ifstream *maskFile) {
            //int tempSum = 0;
            for (int i = 0; i < 25; i++) {
                *maskFile >> maskAry[i];
                cout << "mask arr i :" << i << " content: " << maskAry[i] << endl;
                //tempSum += maskAry[i];
                maskWeight += maskAry[i];
            }
            //cout << tempSum << endl;
        }

        void loadNeighborAry(int** mirrorFramedAry, int i, int j, int* neighborAry) {
            int rowIndex, colIndex, neighborIndex = 0;

            // loop through 5x5 matrix of neighbor cells
            for (int row = i - 2; row <= i + 2; row++) {
                for (int col = j - 2; col <= j + 2; col++) {
                    rowIndex = row;
                    colIndex = col;

                    // handle out-of-bounds cases using mirror-framing
                    if (row < 0) {
                        rowIndex = -row;
                    } else if (row >= numRows) {
                        rowIndex = (numRows - 1) - (row - (numRows - 1));
                    }
                    if (col < 0) {
                        colIndex = -col;
                    } else if (col >= numCols) {
                        colIndex = (numCols - 1) - (col - (numCols - 1));
                    }

                    // add neighbor value to array
                    neighborAry[neighborIndex] = mirrorFramedAry[rowIndex][colIndex];
                    neighborIndex++;
                }
            }
        }

        void computeMedian() {
            int i = 2;
            int j = 2;
            while (i < numRows + 2) {
                j = 2;
                while (j < numCols +2) {
                    loadNeighborAry(mirrorFramedAry, i, j, neighborAry);
                    sort(neighborAry, neighborAry + 25);
                    medianAry[i][j] = neighborAry[12];
                    j++;
                }
                i++;
            }
        }

        void computeAvg() {
            for (int i = 2; i < numRows + 2; i++) {
                for (int j = 2; j < numCols + 2; j++) {
                    int sum = 0;
                    for (int k = i - 2; k <= i + 2; k++) {
                        for (int l = j - 2; l <= j + 2; l++) {
                            sum += mirrorFramedAry[k][l];
                        }
                    }
                    int avg = sum / 25;
                    avgAry[i-2][j-2] = avg;
                }
            }
        }

        void computeGauss() {
            int i = 2;
            while (i < numRows + 2) {
                int j = 2;
                while (j < numCols + 2) {
                    loadNeighborAry(mirrorFramedAry, i, j, neighborAry);
                    gaussAry[i][j] = convolution();
                    j++;
                }
                i++;
            }
        }

        int convolution() {
            int result = 0;
            int i = 0;
            while (i < 25) {
                result += neighborAry[i] * maskAry[i];
                i++;
            }
            //cout << maskWeight << " MASKWEIGHT" << endl;
            return (result/maskWeight);
        }

        void imgReformat(ofstream *outFile, int** inAry) {
            *outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
            string strMaxVal = to_string(maxVal);
            int Width = strMaxVal.length();
            for (int r = 2; r < numRows + 2; r++) {
                for (int c = 2; c < numCols + 2; c++) {
                    *outFile << inAry[r][c] << " ";
                    string strPixel = to_string(inAry[r][c]);
                    int WW = strPixel.length();
                    *outFile << string(Width - WW, ' ');
                }
                *outFile << endl;
            }
        }

        void prettyPrint(int** Ary, ofstream *outFile) {
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (Ary[i][j] > 0) {
                        *outFile << Ary[i][j] << "  ";
                    } else {
                        *outFile << " " ;
                    }
                }
                *outFile << "\n";
            }
        }

};


int main(int argc, char** argv) {
    ifstream inFile(argv[1]);
    ifstream maskFile(argv[2]);
    int thrVal = atoi(argv[3]);
    ofstream avgOutFile(argv[4]);
    ofstream medianOutFile(argv[5]);
    ofstream gaussOutFile(argv[6]);
    ofstream debugOutFile(argv[7]);
    ofstream imgOutFile(argv[8]);
    int numRows, numCols, minVal, maxVal;
    int maskRows, maskCols, maskMin, maskMax;
    inFile >> numRows >> numCols >> minVal >> maxVal;
    maskFile >> maskRows >> maskCols >> maskMin >> maskMax;
    Enhancement enhanceObj(numRows, numCols, minVal, maxVal, 
                            maskRows, maskCols, maskMin, maskMax);
    enhanceObj.thrVal = atoi(argv[3]);
    cout << "Step 0 1 2: " << " THR: " << enhanceObj.thrVal 
            << " enhanceObj:numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax "
            << enhanceObj.numRows << " " << enhanceObj.numCols << " " << enhanceObj.minVal << " " << enhanceObj.maxVal
            << " " << enhanceObj.maskRows << " " << enhanceObj.maskCols << " " << enhanceObj.maskMin << " " << enhanceObj.maskMax
            << endl; // Step 0 & Step 1 & Step 2
    
    enhanceObj.loadMaskAry(&maskFile); // step 3
    enhanceObj.loadImage(&inFile); // step 4
    enhanceObj.mirrorFraming(); // step 5
    enhanceObj.imgReformat(&imgOutFile, enhanceObj.mirrorFramedAry); // step 6
    cout << " Step 3 - 6" << endl;

    enhanceObj.computeAvg();
    enhanceObj.imgReformat(&imgOutFile, enhanceObj.avgAry);
    enhanceObj.binaryThreshold(enhanceObj.avgAry);
    enhanceObj.prettyPrint(enhanceObj.thrAry, &avgOutFile); // step 7

    enhanceObj.computeMedian();
    enhanceObj.imgReformat(&imgOutFile, enhanceObj.medianAry);
    enhanceObj.binaryThreshold(enhanceObj.medianAry);
    enhanceObj.prettyPrint(enhanceObj.thrAry, &medianOutFile); // step8
    cout << "done with median " << endl;

    enhanceObj.computeGauss();
    enhanceObj.imgReformat(&imgOutFile, enhanceObj.gaussAry);
    enhanceObj.binaryThreshold(enhanceObj.gaussAry);
    enhanceObj.prettyPrint(enhanceObj.thrAry, &gaussOutFile); // step 9

    inFile.close();
    maskFile.close();
    avgOutFile.close();
    medianOutFile.close();
    gaussOutFile.close();
    debugOutFile.close();
    imgOutFile.close(); // step 10
    cout << " Full execution" << endl;

    return 0;
}