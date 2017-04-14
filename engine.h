#ifndef ENGINE_H
#define ENGINE_H

typedef unsigned char msize;

void random1(int n, msize matrix[n][n]);
void clear(int n, msize matrix[n][n]);
void preset(int n, msize matrix[n][n]);
int replicator(int neighbours, int state);
int life(int neighbours, int state);
int neigh(int n, msize matrix[n][n], int row, int col);
void engine(int n, msize matrix[n][n]);
void copyMatrix(int n, msize matrixA[n][n], msize matrixB[n][n]);
#endif

