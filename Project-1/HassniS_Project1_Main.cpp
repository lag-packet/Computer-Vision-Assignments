#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class ThresholdSelection {
    public:
        int numRows, numCols, minVal, maxVal;
        int maxHeight;
        int x1, y1, x2, y2;
        int* histAry;
        int deepestThrVal;
        int biGaussThrVal;
        int* gaussAry;
        ThresholdSelection() {
            cout << "creating threshold object" << endl;
            //int counter = 0;
            histAry = new int[maxVal + 1];
            for (int i = 0; i < maxVal; i++) {
                histAry[i] = 0;
                //counter ++;
            }
            gaussAry = new int[maxVal + 1];
            for (int i = 0; i < maxVal; i++) {
                gaussAry[i] = 0;
                //counter ++;
            }
            //cout << "COUNTER: " << counter << endl;
        }
        int loadHist(ifstream *inFile) {
            int currentIndex;
            while (*inFile >> currentIndex) {
                int value;
                *inFile >> value;
                histAry[currentIndex] = value;
                // cout << "index " << currentIndex << " has value " << histAry[currentIndex] << endl;
            }
            int maxInt = 0;
            for (int i = 0; i < maxVal; i++) {
                if (maxInt < histAry[i]) {
                    maxInt = histAry[i];
                }
            }
            maxHeight = maxInt;
            return maxHeight;
        }
        void dispHist(ofstream *outFile1) {
            *outFile1 << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
            for (int i = minVal; i <= maxVal; i++) {
                *outFile1 << i << " (" << histAry[i] << "):";
                for (int j = 0; j < histAry[i]; j++) {
                    *outFile1 << "+";
                }
                *outFile1 << endl;
            }
        }
        void setZero(int *ary, int size) {
            for (int i = 0; i < size; i++) {
                ary[i] = 0;
            }
        }
        int deepestConcavity(ofstream *deBugFile) {
            *deBugFile << "Entering deepestConcavity method" << endl;
            double m = (double) (y2 - y1) / (double) (x2- x1);
            double b = (double) y1 - (m * (double) x1);
            int maxGap = 0;
            int first = x1;
            int second = x2;
            int x = first;
            int thr = first;// step 0
            while (x <= second) {
                int y = (int) (m * x + b);
                int gap = (abs) (histAry[x] - y);
                if (gap > maxGap) {
                    maxGap = gap;
                    thr = x;
                }
                x++;
            }
            *deBugFile << "leaving deepestConcavity method, maxGap is and thr is ;" << maxGap << " " << thr << endl;
            return thr;
        }
        int biGauss(ofstream *deBugFile) {
            *deBugFile << "Entering biGaussian method" << endl;
            double sum1;
            double sum2;
            double total;
            double minSumDiff;
            int offSet = (int) (maxVal - minVal) / 10;
            int dividePt = offSet; 
            int bestThr = dividePt;
            minSumDiff = 999999.0; //step 0
            while (dividePt < (maxVal - offSet)) {
                setZero(gaussAry, maxVal); // step 1
                sum1 = fitGauss(0, dividePt, deBugFile); // step 2
                sum2 = fitGauss(dividePt, maxVal, deBugFile); // step 3
                double total = sum1 + sum2; // step 4
                if (total < minSumDiff) {
                    minSumDiff = total;
                    bestThr = dividePt;
                } // step 5
                *deBugFile << "divpt: " << dividePt << " sum1: " << sum1 << " sum2: " << sum2 << " total: " << total << " minsumdiff: " << minSumDiff
                << " besthr: " << bestThr << endl; // step 6
                dividePt++; // step 7
            }
            *deBugFile << "leaving biguass method, minSumDiff = " << minSumDiff << " best thr is" << bestThr << endl;
            return bestThr;
        }
        double computeMean(int leftIndex, int rightIndex, ofstream* deBugFile) {
            *deBugFile << "Entering Compute Mean method" << endl;
            int refMax = 0;
            int sum = 0;
            int numPixels = 0; // step 0
            int index = leftIndex; // step 1
            sum += (histAry[index] * index);
            while (index < rightIndex) {
                numPixels += histAry[index]; // step 2
                if (histAry[index] > refMax) {
                    maxHeight = histAry[index];
                } // step 3
                index++; // step 4
            } // step 5
            double result = (double) sum / (double) numPixels; // step 6
            *deBugFile << "Leaving computeMean mehtod maxHeight is an result " << maxHeight << " " << result << endl;
            return result;
        }
        double computeVar(int leftIndex, int rightIndex, double mean, ofstream* deBugFile) {
            *deBugFile << "Entering Compute Var Method " << endl;
            double sum = 0.0;
            double numPixels = 0.0; // step 0
            int index = leftIndex; // step 1
            while (index < rightIndex) {
                sum += (double) histAry[index] * pow((double) index - mean, 2);
                numPixels += histAry[index]; // step 2
                index++; // step 3
            } // step 4
            double result = sum / (double) numPixels; // step 5
            *deBugFile << "Leaving computeVar Method returning result " << result << endl;
            return result;
        }
        double modifiedGauss (int x, double mean, double var) {
            return  (double) (maxHeight * exp ( - ( ((double) x-mean)*((double) x-mean) / (2*var) ) ) );
        }
        double fitGauss(int leftIndex, int rightIndex, ofstream* deBugFile) {
            *deBugFile << "Entering Compute fit guass" << endl;
            double mean;
            double var;
            double sum = 0.0;
            double gVal;
            double maxGVal; // step 0
            mean = computeMean(leftIndex, rightIndex, deBugFile);
            var = computeVar(leftIndex, rightIndex, mean, deBugFile); // step1
            int index = leftIndex; // step 2
            while (index <= rightIndex) {
                gVal = modifiedGauss(index, mean, var); // step 3
                sum += abs(gVal - (double) histAry[index]); // step 4
                gaussAry[index] = gVal; // step 5
                index++; // step 6
            }
            *deBugFile << "LEaving fitguass sum: " << sum << endl;
            return sum;
        }
};

int main(int argc, char** argv) {
    cout << "Running" << endl;
    ifstream inFile1(argv[1]); 
    ifstream inFile2(argv[2]);
    ofstream outFile1(argv[3]);
    ofstream deBugFile(argv[4]); // step 0
    ThresholdSelection thsObj;
    inFile1 >> thsObj.numRows >> thsObj.numCols >> thsObj.minVal >> thsObj.maxVal;
    int maxHeight = thsObj.loadHist(&inFile1);
    //cout << "MAx int: " << maxHeight << endl;
    inFile2 >> thsObj.x1 >> thsObj.y1 >> thsObj.x2 >> thsObj.y2;
    cout << "X1 " << thsObj.x1 << "y1 " << thsObj.y1 << "x2 " << thsObj.x2 << "y2 " << thsObj.y2; // step 1
    thsObj.dispHist(&outFile1); // step 2
    int deepestThrVal = thsObj.deepestConcavity(&deBugFile); 
    //cout << "Deepest thrVal " << deepestThrVal << endl;
    outFile1 << "Deepest THR VAL: " << deepestThrVal << endl; // step 3
    int biGaussThrVal = thsObj.biGauss(&deBugFile);
    //cout << " BI GUAS VALL " << biGaussThrVal;
    outFile1 << "biGaussThrVal THR VAL: " << biGaussThrVal << endl; // step 4
    inFile1.close();
    inFile2.close();
    outFile1.close();
    deBugFile.close();
    return 0;
}

