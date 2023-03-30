// Project 01 basic threshold - Computer Vision

import java.io.*;
import java.util.*;

public class HassniS_Project01_Main {
    public static void main(String[] args) {
        int threshold = 0;
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter threshold ");
        threshold = scanner.nextInt();
        processContents(readFile(args[0]), args[1], threshold);
    }

    public static String readFile(String fileName) {
        String data = "";
        try {
            File inFile = new File(fileName);
            Scanner fileReader = new Scanner(inFile);
            while(fileReader.hasNextLine()) {
                data += fileReader.nextLine();
                data += '\n';
            }
            fileReader.close();
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        return data;
    }

    public static void processContents(String contents, String outFile, int threshold) {
        String processedColumn = "";
        Scanner scanner = new Scanner(contents);
        int row = 0;
        while(scanner.hasNextLine()) {
            String data = scanner.nextLine();
            String[] currentRow = data.split("\\s+");
            // Change header min, max
            if (row == 0) {
                currentRow[2] = "0";
                currentRow[3] = "1";
                for (String s : currentRow) { processedColumn += s + ' ';}
                row++;
            }
            // Process the rest of contents
            else {
                for (int i = 0; i < currentRow.length; i++) {
                    if (currentRow[i] == "") {break;}
                    int j = Integer.parseInt(currentRow[i]);
                    if (j >= threshold) {
                        processedColumn += "1" + ' ';
                    } else {
                        processedColumn += "0" + ' ';
                    }
                }
            }
            processedColumn += "\n";
        }
        // write processed Contents content to file
        BufferedWriter out = null;
        try {
            out = new BufferedWriter(new FileWriter(outFile));
            out.write(processedColumn);
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }
}