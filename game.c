#include "game.h"
/*
DRAKE BARTOLAI - MP8: 2048
To complete this MP I wrote a series of functions. These functions each carry out various components in the
classic game 2048. make_game and remake_game are used to setup the gameboard based on the user-input dimensions.
There is one function, cell *get_cell, which returns a pointer to a cell on the gameboard. This is very useful
in all of my move functions. The move functions are used to move the tiles on the gameboard and to cause them to
merge and update the score as necessary. Finally, the check_legal_move checks whether there are any more moves
the player can make or if the game is over.
*/

game * make_game(int rows, int cols) //make a gameboard according to input dimensions
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    	game * mygame = malloc(sizeof(game));
    	mygame->cells = malloc(rows*cols*sizeof(cell));
	mygame->rows = rows; //set the rows
	mygame->cols = cols; //set the columns
	mygame->score = 0; //initialize the score to 0
	for (int i = 0; i < rows*cols; i++){ //initialize cells to -1
		mygame->cells[i] = -1;}

    return mygame; //return the gameboard and values in the struct
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols) //remake the game if necessary
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));
	(*_cur_game_ptr)->rows = new_rows; //reset the rows
	(*_cur_game_ptr)->cols = new_cols; //reset the columns
	(*_cur_game_ptr)->score = 0; //reset the score to 0
	for (int i = 0; i < new_rows * new_cols; i++){ //reset the cells to -1
		(*_cur_game_ptr)->cells[i] = -1;}
	return; //void fn returns nothing
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
	if(row > cur_game->rows || col > cur_game->cols){ //return NULL if the cell is off the gameboard
    	{return NULL;}
	return &cur_game->cells[(row*cur_game->cols) + col]; //return pointer to the cell at row and column specified
}

int move_w(game * cur_game){   //Remember to create variable to make sure the same tile can'r merge twice in one turn
	int ret = 0; //initialize return var to 0
	for (int c = 0; c < cur_game->cols; c++){ //loop thru cols
		int last_merged = -1; //this will help later to assure the same tile doesnt merge twice in one call
		for (int r = 1; r < cur_game->rows; r++){ //loop thru rows
			cell *cur_cell = get_cell(cur_game, r, c); //establish the current cell using get_cell
			if (*cur_cell == -1){continue;} //skip empty cells
			for (int k = r-1; k >= 0; k--){ //use k to move values up as needed
				cell *cell_up = get_cell(cur_game, k, c); //find cell above cell in question
				cell *cur_cell = get_cell(cur_game, k+1, c); //reset cur_cell based on k
				if (*cell_up == -1){ //if the cell above is empty, move cur_cell up and set return var to 1
				*cell_up = *cur_cell;
				*cur_cell = -1;
				ret = 1;}
				else if (*cell_up == *cur_cell && k != last_merged){ //otherwise if the above cell == cur_cell
				*cell_up *= 2; //combine the cells
				*cur_cell = -1; //make cell below empty
				cur_game->score = cur_game->score + *cell_up; //set score accordingly
				last_merged = k; //set last_merged so same cell won't merge twice in one turn
				ret = 1; //return var set to 1 since there is valid move
				break;} //to prevent merging more than once
				else break;}}} //to prevent redundant looping over empty cells
    	return ret; //returns 1 if a valid move occured or 0 otherwise
};

int move_s(game * cur_game){ //For most of the function, commands are very similar to move_w but to move down
	int ret = 0; //same return var behavior
	for (int c = 0; c < cur_game->cols; c++){ //loop thru cols
		int last_merged = -1; //same last_merge behavior
		for (int r = cur_game->rows - 2; r>=0; r--){ //iterate thru rows to go up column instead of down
			cell *cur_cell = get_cell(cur_game, r, c); //find cell
			if (*cur_cell == -1){continue;} //skip empty cell
			for (int k = r+1; k <= cur_game->rows-1; k++){ //use k to find cells below cur_cell instead of above
				cell *cell_down = get_cell(cur_game, k, c); //this change is reflected here
				cell *cur_cell = get_cell(cur_game, k-1, c); //same cur_cell behavior
				if (*cell_down == -1){ //if below cell is empty, move down and return set to 1
				*cell_down = *cur_cell;
				*cur_cell = -1;
				ret = 1;}
				else if (*cell_down == *cur_cell && k != last_merged){ //otherwise, if cell below == cur_cell
				*cell_down *= 2; //combine the cells
				*cur_cell = -1; //make cell empty
				cur_game->score += *cell_down; //set score accordingly
				last_merged = k; //update last_merged
				ret = 1;	//update ret var
				break;} //revents merging more than once
				else break;}}} //prevents redundant iterations
	return ret; //return accordingly
};

int move_a(game * cur_game) //slide left. algorithm is similar to move_w but flip cols and rows
{
	int ret = 0;
	for (int r = 0; r < cur_game->rows; r++){ //start with rows this time
		int last_merged = -1;
		for (int c = 1; c < cur_game->cols; c++){ //iterate right thru cols
			cell *cur_cell = get_cell(cur_game, r, c);
			if (*cur_cell == -1){continue;} //skip empty cells
			for (int k = c-1; k >= 0; k--){ //iterate to the left of cur_cell
				cell *cell_left = get_cell(cur_game, r, k); //find cell to the left
				cell *cur_cell = get_cell(cur_game, r, k+1);
				if (*cell_left == -1){ //if cell to the left is empty, slide the cell
				*cell_left = *cur_cell;
				*cur_cell = -1;
				ret = 1;}
				else if (*cell_left == *cur_cell && k != last_merged){ //merge cells and set score accordingly
				*cell_left *= 2;
				*cur_cell = -1;
				cur_game->score += *cell_left;
				last_merged = k;
				ret = 1;
				break;}
				else break;}}}
	return ret;
};

int move_d(game * cur_game){ //slide to the right. more or less same as the other three fns
	int ret = 0;
	for (int r = 0; r < cur_game->rows; r++){ //loop thru rows
		int last_merged = -1;
		for (int c = cur_game->cols - 2; c >= 0; c--){ //loop thru cols
			cell *cur_cell = get_cell(cur_game, r, c);
			if (*cur_cell == -1){continue;}
			for (int k = c+1; k <= cur_game->cols - 1; k++){ //loop thru cels to the right
				cell *cell_right = get_cell(cur_game, r, k); //get current cell
				cell *cur_cell = get_cell (cur_game, r, k-1); //get cell to the right
				if (*cell_right == -1){ //if empty space to the right, the cell should fill that space
				*cell_right = *cur_cell;
				*cur_cell = -1;
				ret = 1;}
				else if (*cell_right == *cur_cell && k != last_merged){ //if cell to the right can merge, merge and set score
				*cell_right *= 2;
				*cur_cell = -1;
				cur_game->score += *cell_right;
				last_merged = k;
				ret = 1;
				break;}
				else break;}}}
	return ret; //return accordingly
};

int legal_move_check(game * cur_game) //check the gameboard. if there are no legal moves remaining, return 0. otherwise return 1
{
	for (int r = 0; r < cur_game->rows; r++){ //iterate thru rows and columns (thru every cell)
		for (int c = 0; c < cur_game->cols; c++){
			cell *check = get_cell(cur_game, r, c); //need to check each cell for following conditions
			if (*check == -1) return 1; //if the cell is empty, return 1; there is a legal move
			if (r > 0){ //if the row > 0, there is a cell above that could match
				cell *cell_up = get_cell(cur_game, r-1, c); //find the cell above *check
				if (*cell_up == *check) return 1; //if they match, return 1; there's a legal moce
				}
			if (c > 0){ //if the col > 0, there is a cell to the left that could match
				cell *cell_left = get_cell(cur_game, r, c-1); //find cell to left
				if (*cell_left == *check) return 1; //if they match, return 1
				}
			if (r < cur_game->rows - 1){ //same idea with a cell below
				cell *cell_down = get_cell(cur_game, r+1, c);
				if (*cell_down == *check) return 1;
				}
			if (c < cur_game->cols - 1){ //same idea with a cell to the left
				cell *cell_right = get_cell(cur_game, r, c+1);
				if (*cell_right == *check) return 1;
				}}}
return 0; //if each cell makes it through every condition, there are no legal moves. return 0.
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
