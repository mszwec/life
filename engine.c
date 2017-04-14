#include <stdlib.h> 
#include <memory.h> 
#include "engine.h"

void engine(int n, msize matrix[n][n])
{

    int row, col;
    int copy[n][n];
    int neighbours;

    for(row = 0; row<n; row++)
    {
        for(col = 0; col<n; col++)
        {
            neighbours = neigh(n, matrix, row, col);
#ifndef REPLICATOR
            copy[row][col] = life( neighbours, matrix[row][col]);
#else
            copy[row][col] = replicator( neighbours, matrix[row][col]);
#endif
        }	
    }

    for(row = 0; row<n; row++)
    {
        for(col = 0; col<n; col++)
        {
            matrix[row][col] = copy[row][col];
        }
    }
}

int life(int neighbours, int state)
{
    if(state == 1)
    {
        if(neighbours==2 || neighbours==3)
            return 1;				
        else
            return 0;
    }
    else if(neighbours==3)
        return 1;				
    else
        return 0;
}

int replicator(int neighbours, int state)
{

    switch (neighbours)
    {
    case 1:
    case 3:
    case 5:
    case 7:
        return 1;
    }

    return 0;
}

int neigh(int n, msize matrix[n][n], int row, int col)
{
    int neighbours=0;

    for(int i=row-1; i<=row+1; i++)
    {
        if(i<0 || i >= n) continue;

        for(int j=col-1; j<=col+1; j++)
        {
            if(j<0 || j >= n) continue;
            if(i==row && j==col) continue;

            if(matrix[i][j] != 0)
                neighbours++;
        }
    }

    return neighbours;
}



void random1(int n, msize matrix[n][n])
{
    for(int row = 0; row<n; row++)
        for(int col = 0; col<n; col++)
            matrix[row][col] = rand()%2;
}

void clear(int n, msize matrix[n][n])
{
    memset(matrix, (msize)0, sizeof(msize)*n*n);
}

void preset(int n, msize matrix[n][n])
{
    static int presetno;
    int maxpresetno = 2;
    int row, col, myrow, mycol;
    static msize guns[9][36] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    clear(n, matrix);	

    switch(presetno)
    {

    case 0:

        if(n>46)
            for(row = 10, myrow=0; row<19; row++,myrow++)
                for(col = 10, mycol=0; col<46; col++, mycol++)
                    matrix[col][row] = guns[myrow][mycol];
        break;

    case 1:

        for(row = 0; row<n; row++)
            for(col = 0; col<n; col++)
                if(row == 0 || col == 0 || row == n-1 || col == n-1)
                    matrix[col][row] = 1;
        break;
    case 2:

        for(row = 0; row<n; row++)
            for(col = 0; col<n; col++)
                if( col == (int)n/2 || col == (int)(n/2)-1)
                    matrix[col][row] = 1;
        break;
    }

    presetno++;
    if(presetno > maxpresetno)
        presetno = 0;
}

void copyMatrix(int n, msize matrixA[n][n], msize matrixB[n][n])
{
    memcpy(matrixB, matrixA, sizeof(msize)*n*n );
}

