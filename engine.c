 
#include <stdlib.h>
void random1(int n, int matrix[n][n]);
void clear(int n, int matrix[n][n]);
void preset(int n, int matrix[n][n]);
int replicator(int neighbours, int state);
int life(int neighbours, int state);
int neigh(int n, int matrix[n][n], int row, int col);
void engine(int n, int matrix[n][n]);




void engine(int n, int matrix[n][n]){

int row, col;
int copy[n][n];
int neighbours;

	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){
			neighbours = neigh(n, matrix, row, col);
#ifndef REPLICATOR
			copy[row][col] = life( neighbours, (int)matrix[row][col]);
#else
			copy[row][col] = replicator( neighbours, (int)matrix[row][col]);
#endif
		}	
	}

	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){
			matrix[row][col] = copy[row][col];
		}
	}
}

int life(int neighbours, int state){
	if(state == 1){
		if(neighbours==2 || neighbours==3){
			return 1;				
		}else{
			return 0;
		}
	}
	if(state == 0){
		if(neighbours==3){
			return 1;				
		}
	}
	return 0;
}

int replicator(int neighbours, int state){

	switch (neighbours) {
		case 1:
		case 3:
		case 5:
		case 7:
			return 1;
	}

	return 0;
}

int neigh(int n, int matrix[n][n], int row, int col){

int neighbours=0;

			if(row-1>=0 && col-1>=0 && matrix[row-1][col-1]!=0){
					neighbours++;	
			}
			if(row-1>=0 && matrix[row-1][col]!=0){
					neighbours++;	
			}
			if(row-1>=0 && col+1<n && matrix[row-1][col+1]!=0){
					neighbours++;	
			}	
			if(col-1>=0 && matrix[row][col-1]!=0){
					neighbours++;	
			}
			if(col+1<n && matrix[row][col+1]!=0){
					neighbours++;	
			}
			if(row+1<n && col-1>=0 && matrix[row+1][col-1]!=0){
					neighbours++;	
			}
			if(row+1<n && matrix[row+1][col]!=0){
					neighbours++;	
			}
			if(row+1<n && col+1<n && matrix[row+1][col+1]!=0){
					neighbours++;	
			}

return neighbours;
}



void random1(int n, int matrix[n][n]){
int row, col;
	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){
			matrix[row][col] = rand()%2;

			//printf("%d", matrix[row][col]);
		}	
	//printf("\n");
	}
}

void clear(int n, int matrix[n][n]){
int row, col;
	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){
			matrix[row][col] = 0;
			//printf("%d", matrix[row][col]);
		}	
	//printf("\n");
	}
}

void preset(int n, int matrix[n][n]){

static int presetno;
int maxpresetno = 2;
int row, col, myrow, mycol;
static int guns[9][36] = {
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
	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){
			matrix[row][col] = 0;
		}	
	}
	switch(presetno){

		case 0:

			if(n>46){
				for(row = 10, myrow=0; row<19; row++,myrow++){
					for(col = 10, mycol=0; col<46; col++, mycol++){
						matrix[col][row] = guns[myrow][mycol];
					}	
				}
			}
			break;

		case 1:

			for(row = 0; row<n; row++){
				for(col = 0; col<n; col++){
					if(row == 0 || col == 0 || row == n-1 || col == n-1){
						matrix[col][row] = 1;
					}
				}	
			}
			break;
		case 2:

			for(row = 0; row<n; row++){
				for(col = 0; col<n; col++){
					if( col == (int)n/2 || col == (int)(n/2)-1){
						matrix[col][row] = 1;
					}
				}	
			}
			break;
	}
	presetno++;
	if(presetno > maxpresetno){
		presetno = 0;
	}
}
