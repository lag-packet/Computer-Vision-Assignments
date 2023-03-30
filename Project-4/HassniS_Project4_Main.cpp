#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Property {
    int label;      // The component label
    int numPixels;  // Total number of pixels in the connected component
    int minR;       // Minimum row index with respect to the input image
    int minC;       // Minimum column index with respect to the input image
    int maxR;       // Maximum row index with respect to the input image
    int maxC;       // Maximum column index with respect to the input image

    // Default constructor
    Property() {
        label = 0;
        numPixels = 0;
        minR = 0;
        minC = 0;
        maxR = 0;
        maxC = 0;
    }

    // Constructor to initialize the struct properties
    Property(int label, int numPixels, int minR, int minC, int maxR, int maxC) {
        this->label = label;
        this->numPixels = numPixels;
        this->minR = minR;
        this->minC = minC;
        this->maxR = maxR;
        this->maxC = maxC;
    }
};

class ccLabel {
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newLabel;
    int trueNumCC;
    int newMin;
    int newMax;
    int **zeroFramedAry;
    int NonZeroNeighborAry[5];
    int *EQAry;
    char option;
    Property *CCproperty;

    // Constructor to initialize the ccLabel object and dynamically allocate zeroFramedAry and EQAry
    ccLabel(int numRows, int numCols, int minVal, int maxVal, char option) {
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->option = option;

        // Dynamically allocate zeroFramedAry
        zeroFramedAry = new int*[numRows + 2];
        for (int i = 0; i < numRows + 2; i++) {
            zeroFramedAry[i] = new int[numCols+2];
            for (int j = 0; j < numCols + 2; j++) {
                zeroFramedAry[i][j] = 0;
            }
        }

        // Dynamically allocate EQAry and initialize to its index
        EQAry = new int[(numRows * numCols) / 4];
        for (int i = 0; i < (numRows * numCols) / 4; i++) {
            EQAry[i] = i;
        }

        // Initialize other members
        newLabel = 0;
        trueNumCC = 0;
        newMin = 0;
        newMax = trueNumCC;

        // Dynamically allocate CCproperty
        CCproperty = new Property[trueNumCC + 1];
    }

    // Destructor 
    ~ccLabel() {
        for (int i = 0; i < numRows + 2; i++) {
            delete[] zeroFramedAry[i];
        }
        delete[] zeroFramedAry;

        delete[] EQAry;
        delete[] CCproperty;
    }

    // Member functions 
    void zero2D(int** arr, int numRows, int numCols) {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                arr[i][j] = 0;
            }
        }
    }

    void negative1D(int *array, int size) {
        for (int i = 0; i < size; i++) {
            array[i] = -1;
        }
    }

    void loadImage(ifstream *inFile) {
        for (int i = 1; i < numRows + 2; i++) {
            for (int j = 1; j < numCols + 2; j++) {
                *inFile >> zeroFramedAry[i][j];
            }
         }
    }

    void conversion() {
        for (int i = 1; i <= numRows-1; i++) {
            for (int j = 1; j <= numCols+1; j++) { // to change?
                if (zeroFramedAry[i][j] == 0) {
                    zeroFramedAry[i][j] = 1;
                } else {
                    zeroFramedAry[i][j] = 0;
                }
            }
        }
    }

    void connected4Pass1(ofstream *RFprettyPrintFile, ofstream *deBugFile) {
        *deBugFile << "entering connected4 method" << endl;

        // what is image data structure? what is newLabel data structure

        // Step 1
        for (int i = 0; i < numRows; i++) { // scan top -> bottom
            for (int j = 0; j < numCols; j++) { // scan left -> right
                // Step 2
                if (zeroFramedAry[i][j] > 0) {
                    int a = (i > 0) ? zeroFramedAry[i-1][j] : 0;
                    int b = (j > 0) ? zeroFramedAry[i][j-1] : 0;
                    int c = (i > 0 && j > 0) ? zeroFramedAry[i-1][j-1] : 0;
                    int d = 0;
                    if (i > 0 && j < numCols - 1) {
                    d = zeroFramedAry[i-1][j+1];
                    } else {
                    d = 0;
                    }
                    
                    if (a == 0 && b == 0 && c == 0 && d == 0) { // case 1
                        newLabel++;
                        zeroFramedAry[i][j] = newLabel;
                    }
                    else {
                        // remove 0's from labels
                        int labels[4] = {a, b, c, d};
                        int numLabels = 0;
                        for (int k = 0; k < 4; k++) {
                            if (labels[k] != 0) {
                                numLabels++;
                            }
                        }
                        if (numLabels == 0) { // case 1
                            newLabel++;
                            zeroFramedAry[i][j] = newLabel;
                        } else {
                            int minLabel = 9999;
                            for (int k = 0; k < 4; k++) {
                                if (labels[k] != 0 && labels[k] < minLabel) {
                                    minLabel = labels[k];
                                }
                            }
                            zeroFramedAry[i][j] = minLabel;
                            for (int k = 0; k < 4; k++) {
                                if (labels[k] != 0 && labels[k] != minLabel) {
                                    EQAry[labels[k]] = minLabel;
                                }
                            }
                        }
                    }
                    
                    // Step 4
                    while (true) {
                        bool updated = false;
                        for (int label = 1; label <= newLabel; label++) {
                            if (EQAry[label] != 0) { // find root of equivalence class
                                int root = EQAry[label];
                                while (root != EQAry[root]) {
                                    root = EQAry[root];
                                }
                                EQAry[label] = root;
                                updated = true;
                            }
                        }
                        if (!updated) {
                            break;
                        }
                    }
                }
            }
        }
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


};

int main(int argc, char** argv) {
    ifstream inFile(argv[1]);
    int connectness = std::stoi(argv[2]);
    char option = *(argv[3]);

    ofstream RFprettyPrintFile(argv[4]);
    ofstream labelFile(argv[5]);
    ofstream propertyFile(argv[6]);
    ofstream debugFile(argv[7]);

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;

    ccLabel ccLabelObj(numRows, numCols, minVal, maxVal, option);
    cout << "ccLabelObj:" << ccLabelObj.numRows << " " << ccLabelObj.numCols << " " << ccLabelObj.minVal << 
    " " << ccLabelObj.maxVal << " " << ccLabelObj.option << " " << ccLabelObj.newLabel << endl; // step 0

    ccLabelObj.zero2D(ccLabelObj.zeroFramedAry, ccLabelObj.numRows + 2, ccLabelObj.numCols + 2); // step 1
    ccLabelObj.loadImage(&inFile); // step 2
        for (int i = 0; i < numRows + 2; i++) {
            for (int j = 0; j < numCols + 2; j++) {
                cout << ccLabelObj.zeroFramedAry[i][j];
            }
            cout << endl;
        }
    cout << "step 2 debug" << endl;
    
    if (option == 'y' || option == 'Y') {
        cout << "Y conversion" << endl;
        ccLabelObj.conversion();
        cout << endl;
    }

        for (int i = 0; i < numRows + 2; i++) {
            for (int j = 0; j < numCols + 2; j++) {
                cout << ccLabelObj.zeroFramedAry[i][j];
            }
            cout << endl;
        }
    cout << "step 3 debug" << endl;

    if (connectness == 4) {
        ccLabelObj.connected4(&RFprettyPrintFile, &debugFile);
    }
    
    return 0;
}