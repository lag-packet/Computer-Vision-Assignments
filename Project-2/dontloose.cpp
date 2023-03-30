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
                for (int j = 8; j > 5; j--) {
                    mirrorFramedAry[i][j] = topRightCorner;
                }
            }
            // bottom left
            for (int i = numRows + 4; i > numRows; i--) {
                for (int j = 0; j < 3; j++) {
                    mirrorFramedAry[i][j] = bottomLeftCorner;
                }
            }
            // bottom right
            for (int i = numRows + 4; i > numRows; i--) {
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