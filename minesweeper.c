#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <io.h>

#define BOARD_SIZE              10
#define SQUARE_NUM              (BOARD_SIZE*BOARD_SIZE)
#define MINE_NUM                17
#define SHUFFLE_NUM             100000

int mine_board[BOARD_SIZE][BOARD_SIZE];
/* 0 : non-mined, 1 : mined */

int display_board[BOARD_SIZE][BOARD_SIZE];
/* -1 : no mines around, 0 : unknown, 1~8 : number of mines */

void init_board(); // initialize mine_board by randomly planting fixed number of mines
void show_interface(); // print display_board
int sweep(int x, int y);
/*
 * sweep a square on (x, y). if there is no mines around square on (x, y), recursively sweep adjacent squares
 * return : 1 if player sweeps mined square, else 0
 */

int check_game(); 
/*
 * check if the player swept all non-mined squares
 * return : 1 if player swept all non-mined squares, else 0
 */
int checking_board [BOARD_SIZE][BOARD_SIZE];
int main(void)
{
        // TODO
        init_board();
        show_interface();
        
        int y, x =0;
        int i ,j;
        for(i = 0; i < BOARD_SIZE; i++){
        	for(j =0; j < BOARD_SIZE; j++){
        		checking_board[i][j] = count_neighbors(i,j);
			}
		}
		int t= 0;
		while(check_game() == 1){
        printf("input coordinate: ");
        scanf("%d", &x);
        scanf("%d", &y);
       
        if(x > 0 && y > 0 ){
        	if(sweep(x,y) == 1){
        		show_interface();
			}else{
				printf("Game Over");
				return 0;
			}
        	
		}
        system("pause");
		}
        return 0;
      
}

void init_board()
{
        int i;
        int shuffle[BOARD_SIZE * BOARD_SIZE];
        int temp;
        int r1, r2;

        srand(time(NULL)); // set seed

        // initialize shuffle array
        for(i=0; i<SQUARE_NUM; i++)
                shuffle[i] = i;

        // shuffling
        for(i=0; i<SHUFFLE_NUM; i++)
        {
                r1 = rand() % SQUARE_NUM;
                r2 = rand() % SQUARE_NUM;

                temp = shuffle[r1];
                shuffle[r1] = shuffle[r2];
                shuffle[r2] = temp;
        }

        // get mine coordinates from shuffled array
        for(i=0; i<MINE_NUM; i++)
                mine_board[shuffle[i]/BOARD_SIZE][shuffle[i]%BOARD_SIZE] = 1;
}

void show_interface()
{
        int i, j;

        system("clear"); // clear the screen

        // rest of this function just prints out display_board
        printf("    ");
        for(i=0; i<BOARD_SIZE; i++)
                printf(" %2d ", i+1);
        printf("-> X\n");

        for(i=0; i<BOARD_SIZE; i++)
        {
                printf("\n %2d ", i+1);

                for(j=0; j<BOARD_SIZE; j++)
                {
                        if(display_board[i][j] == -1)
                        {
                                if(mine_board[i][j] == 1)
                                        printf("  * ");
                                else
                                        printf("  X ");
                        }
                        else if(display_board[i][j] == 0)
                                printf("  - ");
                        else
                                printf("  %d ", display_board[i][j]);
                }
                printf("\n");
        }
        printf("\n  |\n  v\n\n  Y\n\n");
}

int sweep(int x, int y)
{
	x = x-1;
	y = y-1;
    if(mine_board[x][y] == 1){
    	return 0;
	}
	reveal(x,y,0);
    return 1;
}
void reveal (int i, int j,int flag){
	if(checking_board[i][j] == 0 && flag == 1){
		display_board[i][j] = -1;
	}else{
		display_board[i][j] = checking_board[i][j];
	}

	if(checking_board[i][j] == 0){
		check_nieghbors(i,j);
	}
}
void check_nieghbors(int x, int y){
	int i, j;
	for(i = -1 ; i <= 1 ; i++){
		for(j = -1 ; j <= 1 ; j++){
			int xPos = x + i;
			int yPos = y + j;
			if(xPos > -1 && xPos < BOARD_SIZE && yPos > -1 && yPos < BOARD_SIZE){
				if(checking_board[xPos][yPos] != -1 && display_board[xPos][yPos] == 0){
					reveal(xPos,yPos,1);
				}	
			}
		}	
	}
}
int count_neighbors(int x, int y){
	if(mine_board[x][y] == 1 ){
		return -1;
	}

	int total = 0;
	int i, j;
	for( i= -1 ; i <= 1 ; i++){
		for(j = -1 ; j <= 1 ; j++){
			int xPos = x + i;
			int yPos = y + j;
			if(xPos > -1 && xPos < BOARD_SIZE && yPos > -1 && yPos < BOARD_SIZE){
				if(mine_board[xPos][yPos] == 1){
					total++;
				}
			}
		}	
	}

	return total;
}
int check_game()
{
	int i,j,response = 0;
     for(i = 0; i < BOARD_SIZE; i++){
        	for(j =0; j < BOARD_SIZE; j++){
        		if(mine_board[i][j] != 1 && display_board[i][j] == 0){
        			response = 1;
				}
			}
		}
    return response;
}
