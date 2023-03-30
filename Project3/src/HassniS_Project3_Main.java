import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class HassniS_Project3_Main {

    static class Morphology {
        int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin,
                colOrigin;
        int rowFrameSize, colFrameSize, extraRows, extraCols;
        int rowSize, colSize;
        int[][] zeroFramedAry, morphAry, tempAry, structAry;

        public Morphology(int numImgRows, int numImgCols, int imgMin, int imgMax, int numStructRows,
                          int numStructCols, int structMin, int structMax, int rowOrigin, int colOrigin) {
            this.numImgRows = numImgRows;
            this.numImgCols = numImgCols;
            this.imgMin = imgMin;
            this.imgMax = imgMax;
            this.numStructRows = numStructRows;
            this.numStructCols = numStructCols;
            this.structMin = structMin;
            this.structMax = structMax;
            this.rowOrigin = rowOrigin;
            this.colOrigin = colOrigin;

            this.rowFrameSize = numStructRows / 2;
            this.colFrameSize = numStructCols / 2;
            this.extraRows = rowFrameSize * 2;
            this.extraCols = colFrameSize * 2;

            this.rowSize = numImgRows + extraRows;
            this.colSize = numImgCols + extraCols;

            this.zeroFramedAry = new int[rowSize][colSize];
            this.morphAry = new int[rowSize][colSize];
            this.tempAry = new int[rowSize][colSize];
            this.structAry = new int[numStructRows][numStructCols]; //step 2

            zero2DAry(zeroFramedAry, rowSize, colSize); // step 3
            zero2DAry(morphAry, rowSize, colSize);
            zero2DAry(tempAry, rowSize, colSize);
            zero2DAry(structAry, numStructRows, numStructCols); // step 6

        }

        void copyArys(int[][] morphAry, int[][] zeroFramedAry) {
            System.out.println("copying array");
            for (int i = 0; i < rowSize; i++) {
                for (int j = 0; j < colSize; j++) {
                    zeroFramedAry[i][j] = morphAry[i][j];
                }
            }
        }

        void zero2DAry(int[][] ary, int nRows, int nCols) {
            for (int i = 0; i < nRows; i++) {
                for (int j = 0; j < nCols; j++) {
                    ary[i][j] = 0;
                }
            }
            /*
            System.out.println("Printing zero'd arry");
            for (int i = 0; i < nRows; i++) {
                for (int j = 0; j < nCols; j++) {
                    System.out.print(ary[i][j]);
                }
                System.out.println();
            }
             */
        }

        void loadImg(BufferedReader br) throws IOException {
            // load imgFile to zeroFramedAry inside of frame, begins at (rowOrigin, colOrigin). On your own!
            //System.out.printf("row origin=%d row size=%d col origin=%d col size=%d%n", rowOrigin, rowSize, colOrigin, colSize);
            String line;
            int row = rowOrigin;
            while ((line = br.readLine()) != null) {
                String[] tokens = line.trim().split("\\s+");
                for (int col = colOrigin; col < colOrigin + tokens.length; col++) {
                    zeroFramedAry[row][col] = Integer.parseInt(tokens[col - colOrigin]);
                }
                row++;
            }


            for (int i = 0; i < rowSize; i++) {
                for (int j = 0; j < colSize; j++) {
                    System.out.print(zeroFramedAry[i][j]);
                }
                System.out.println();
            }
        }

        void loadStruct(BufferedReader br) throws IOException {
            // load structFile to structAry. On your own!
            for (int i = 0; i < numStructRows; i++) {
                String line = br.readLine();
                String[] values = line.split(" ");
                for (int j = 0; j < numStructCols; j++) {
                    structAry[i][j] = Integer.parseInt(values[j]);
                }
            }
            /*
            for (int i = 0; i < numStructRows; i++) {
                for (int j = 0; j < numStructCols; j++) {
                    System.out.print(structAry[i][j]);
                }
                System.out.println();
            }
             */
        }

        void computeDilation(int[][] inAry, int[][] outAry, int[][] structAry) {
            // see algorithm below.
            System.out.println("Doing computeDilation");
            int i = rowFrameSize;
            while (i < rowSize) {
                int j = colFrameSize;
                while (j < colSize) {
                    if (inAry[i][j] > 0) {
                        //System.out.println("one pixel dilation ran ");
                        onePixelDilation(i, j, inAry, outAry, structAry); // only processing one pixel inAry[i][j]
                    }
                    j++;
                    //System.out.println("col dial computed");
                }
                //System.out.println("row dial computed");
                i++;
            }
        }

        void computeErosion(int[][] inAry, int[][] outAry, int[][] structAry) {
            // Apply erosion to every pixel in zeroFramedAry. // see algorithm below
            System.out.println("Doing computeErosion");
            int i = rowFrameSize;
            while (i < rowSize) {
                int j = colFrameSize;
                while (j < colSize) {
                    if (inAry[i][j] > 0) {
                        //System.out.println("one pixel erosion ran ");
                        onePixelErosion(i, j, inAry, outAry, structAry);
                    }
                    j++;
                }
                i++;
            }

        }

        void computeOpening(int[][] zeroFramedAry, int[][] morphAry, int[][] structAry, int[][] tempAry) {
            // apply erosion follow by dilation, see algorithm below.
            System.out.println("Doing computeOpening");
            computeErosion(zeroFramedAry, tempAry, structAry);
            computeDilation(tempAry, morphAry, structAry);
        }

        void computeClosing(int[][] zeroFramedAry, int[][] morphAry, int[][] structAry, int[][] tempAry) {
            // apply dilation follow by erosion, see algorithm below.
            System.out.println("Doing computeClosing");
            computeDilation(zeroFramedAry, tempAry, structAry);
            computeErosion(tempAry, morphAry, structAry);
        }

        void onePixelDilation(int i, int j, int[][] inAry, int[][] outAry, int[][] structAry) {
            // / Perform dilation on pixel (i, j) with structAry. See algorithm below
            //System.out.println("doing onePixeldialtion");
            int iOffset = i - rowOrigin;
            int jOffset = j - colOrigin;
            //translation of image coordinate (i,j) with respected origin of the strct elem
            int rIndex = 0;
            while (rIndex < numStructRows) {
                //System.out.println("first while loop");
                int cIndex = 0;
                while (cIndex < numStructCols) {
                    //System.out.println("second while loop");
                    if (structAry[rIndex][cIndex] > 0) {
                        // TO REMOVE
                        if (jOffset + cIndex == colSize) {
                            return;
                        }
                        if (iOffset + cIndex == rowSize) {
                            return;
                        }
                        //System.out.println("i: " + (iOffset + rIndex) + " C: " + (jOffset + cIndex));
                        //System.out.println(rowSize + "  " + colSize);
                        outAry[iOffset + rIndex][jOffset + cIndex] = 1;
                        //System.out.println("dialated column");
                    }
                    cIndex++;
                }
                //System.out.println("dialated row");
                rIndex++;
            }
        }

        void onePixelErosion(int i, int j, int[][] inAry, int[][] outAry, int[][] structAry) {
            // Perform erosion on pixel (i, j) with structAry. See algorithm below.
            int iOffset = i - rowOrigin;
            int jOffset = j - colOrigin;
            boolean matchFlag = true;

            int rIndex = 0;
            while (matchFlag == true && rIndex < numStructRows) {
                //System.out.println("first while");
                int cIndex = 0;
                while (matchFlag == true && cIndex < numStructCols) {
                    //System.out.println("second while");
                    //TO REMOVE
                    if (jOffset + cIndex == colSize) {
                        return;
                    }
                    if (iOffset + rIndex == rowSize) {
                        return;
                    }
                    if (structAry[rIndex][cIndex] > 0 && inAry[iOffset + rIndex][jOffset + cIndex] <= 0) {
                        matchFlag = false;
                    }
                    cIndex++;
                }
                rIndex++;
            }
            if (matchFlag == true) {
                //System.out.println("out ary 1 for compute erosion");
                outAry[i][j] = 1;
            } else {
                //System.out.println("out ary 0 for compute erosion");
                outAry[i][j] = 0;
            }
        }

        void basicOperations(int[][] zeroFramedAry, int[][] morphAry, int[][] structAry, int[][] tempAry,
                             BufferedWriter bw) {
            try {
                System.out.println("doing basic operation");
                bw.write("Entering basicOperations method" + System.lineSeparator());
                zero2DAry(morphAry, rowSize, colSize);
                computeDilation(zeroFramedAry, morphAry, structAry);
                bw.write("Printing result of computeDilation" + System.lineSeparator());
                prettyPrint(morphAry, bw);


                zero2DAry(morphAry, rowSize, colSize);
                computeErosion(zeroFramedAry, morphAry, structAry);
                bw.write("Printing result of computeErosion" + System.lineSeparator());
                prettyPrint(morphAry, bw);


                zero2DAry(morphAry, rowSize, colSize);
                computeOpening(zeroFramedAry, morphAry, structAry, tempAry);
                bw.write("Printing result of computeOpening" + System.lineSeparator());
                prettyPrint(morphAry, bw);


                zero2DAry(morphAry, rowSize, colSize);
                computeClosing(zeroFramedAry, morphAry, structAry, tempAry);
                bw.write("Printing result of computeClosing" + System.lineSeparator());
                prettyPrint(morphAry, bw);

                bw.write("Exit basicOperation method" + System.lineSeparator());
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        void complexOperations(int[][] zeroFramedAry, int[][] morphAry, int[][] structAry, int[][] tempAry,
                               BufferedWriter bw) {
            System.out.println("Doing complex operations");
            try {
                bw.write("entering complexOperations method" + System.lineSeparator());

                zero2DAry(morphAry, rowSize, colSize);
                computeOpening(zeroFramedAry, morphAry, structAry, tempAry);
                bw.write("Pretty print the result of Opening" + System.lineSeparator());
                prettyPrint(morphAry, bw);
                System.out.println("BEFORE COPY");
                copyArys(morphAry, zeroFramedAry);

                zero2DAry(morphAry, rowSize, colSize);
                computeClosing(zeroFramedAry, morphAry, structAry, tempAry);
                bw.write("Pretty print the result of Opening follow by closing" + System.lineSeparator());
                prettyPrint(morphAry, bw);
                copyArys(morphAry, zeroFramedAry);

                zero2DAry(morphAry, rowSize, colSize);
                computeClosing(zeroFramedAry, morphAry, structAry, tempAry);
                bw.write("Pretty print the result of Closing" + System.lineSeparator());
                prettyPrint(morphAry, bw);
                copyArys(morphAry, zeroFramedAry);

                zero2DAry(morphAry, rowSize, colSize);
                computeOpening(zeroFramedAry, morphAry, structAry, tempAry);
                bw.write("Pretty print the result of Closing follow by Opening" + System.lineSeparator());
                prettyPrint(morphAry, bw);

                bw.write("Exit complexOperations method" + System.lineSeparator());
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        void imgReformat(int[][] inAry, BufferedWriter outFile) {
            // re-use code from your previous project or see project 2 specs.
            try {
                outFile.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + System.lineSeparator());
                String str = Integer.toString(imgMax);
                int Width = str.length();
                int r = 1;
                while (r < (numImgRows + 1)) {
                    int c = 1;
                    while (c < (numImgCols + 1)) {
                        str = Integer.toString(inAry[r][c]);
                        outFile.write(str);
                        int WW = str.length();
                        while (WW < Width) {
                            outFile.write(" ");
                            WW++;
                        }
                        c++;
                    }
                    r++;
                    outFile.write(System.lineSeparator());
                }
                outFile.flush();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        void prettyPrint(int[][] ary, BufferedWriter outFile) {
            // outFile can be outFile1 or outFile2.
            // Remark: use “Courier new” font and small font size to fit in a page.
            // if Ary [i, j] == 0 output “. ” // a period follows by a blank
            // else output “1 ” // 1 follows by one blan
            try {
                //outFile.write("Pretty Print" + System.lineSeparator());
                outFile.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + System.lineSeparator());
                for (int i = 0; i < numImgRows + 2; i++) {
                    for (int j = 0; j < numImgCols + 2; j++) {
                        //System.out.println("i :" + i + " " + j);
                        if (ary[i][j] > 0) {
                            outFile.write(ary[i][j] + " ");
                        } else {
                            outFile.write(". ");
                        }
                    }
                    outFile.write(System.lineSeparator());
                }
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        void prettyPrint2(int[][] ary, BufferedWriter outFile) {
            // outFile can be outFile1 or outFile2.
            // Remark: use “Courier new” font and small font size to fit in a page.
            // if Ary [i, j] == 0 output “. ” // a period follows by a blank
            // else output “1 ” // 1 follows by one blan
            try {
                outFile.write(numStructRows + " " + numStructCols + " " + structMin + " " + structMax
                        + System.lineSeparator());
                for (int i = 0; i < numStructRows; i++) {
                    for (int j = 0; j < numStructCols; j++) {
                        //System.out.println("i :" + i + " " + j);
                        if (ary[i][j] > 0) {
                            outFile.write(ary[i][j] + " ");
                        } else {
                            outFile.write(". ");
                        }
                    }
                    outFile.write(System.lineSeparator());
                }
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    public static void main(String[] args) {
        String imgFilePath = args[0];
        String elmFilePath = args[1];
        String outFile1Path = args[2];
        String outFile2Path = args[3];

        int numImgRows = 0, numImgCols = 0, imgMin = 0, imgMax = 0, numStructRows = 0, numStructCols = 0,
                structMin = 0, structMax = 0, rowOrigin = 0, colOrigin = 0;

        try {
            BufferedReader imgReader = new BufferedReader(new FileReader(imgFilePath));
            BufferedReader elmReader = new BufferedReader(new FileReader(elmFilePath));
            BufferedWriter outFile1Writer = new BufferedWriter(new FileWriter(outFile1Path + ".txt"));
            BufferedWriter outFile2Writer = new BufferedWriter(new FileWriter(outFile2Path + ".txt")); // step 0

            String line = imgReader.readLine();
            String[] values = line.split(" ");
            numImgRows = Integer.parseInt(values[0]);
            numImgCols = Integer.parseInt(values[1]);
            imgMin = Integer.parseInt(values[2]);
            imgMax = Integer.parseInt(values[3]);
            System.out.printf("img:" + "%d %d %d %d%n", numImgRows, numImgCols, imgMin, imgMax);

            line = elmReader.readLine();
            values = line.split(" ");
            numStructRows = Integer.parseInt(values[0]);
            numStructCols = Integer.parseInt(values[1]);
            structMin = Integer.parseInt(values[2]);
            structMax = Integer.parseInt(values[3]);
            line = elmReader.readLine();
            values = line.split(" ");
            rowOrigin = Integer.parseInt(values[0]);
            colOrigin = Integer.parseInt(values[1]); // step 1
            System.out.printf("struct:" + "%d %d %d %d%n[%d %d]%n", numStructRows, numStructCols, structMin, structMax,
                    rowOrigin, colOrigin);

            Morphology morhpObj = new Morphology(numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols,
                    structMin, structMax, rowOrigin, colOrigin); // step 2 & 3
            morhpObj.loadImg(imgReader); // step 4
            outFile1Writer.write("Original Image" + System.lineSeparator());
            morhpObj.imgReformat(morhpObj.zeroFramedAry, outFile1Writer);
            outFile1Writer.write("Pretty Print original image" + System.lineSeparator());
            morhpObj.prettyPrint(morhpObj.zeroFramedAry, outFile1Writer); // step 5

            morhpObj.zero2DAry(morhpObj.structAry, morhpObj.numStructRows, morhpObj.numStructCols);
            morhpObj.loadStruct(elmReader);
            outFile1Writer.write("Structuring Element" + System.lineSeparator());
            morhpObj.prettyPrint2(morhpObj.structAry, outFile1Writer); // step 6

            morhpObj.basicOperations(morhpObj.zeroFramedAry, morhpObj.morphAry, morhpObj.structAry, morhpObj.tempAry,
                    outFile1Writer); // step 7
            morhpObj.complexOperations(morhpObj.zeroFramedAry, morhpObj.morphAry, morhpObj.structAry, morhpObj.tempAry,
                    outFile2Writer); // step 8


            outFile1Writer.close();
            outFile2Writer.close(); // step 9
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}