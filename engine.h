#ifndef ENGINE_H
#define ENGINE_H


void random1(int n, int matrix[n][n]);
void clear(int n, int matrix[n][n]);
void preset(int n, int matrix[n][n]);
int replicator(int neighbours, int state);
int life(int neighbours, int state);
int neigh(int n, int matrix[n][n], int row, int col);
void engine(int n, int matrix[n][n]);
void copyMatrix(int n, int matrixA[][n], int matrixB[][n]);
#endif

