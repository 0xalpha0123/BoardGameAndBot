#include <cstdlib>

#include <iostream>

#include <conio.h>

#include <string.h>

#include <time.h>

#define RESET "\033[0;0m"
#define BLACK "\033[0;30m" /* Black */
#define RED "\033[0;31m" /* Red */
#define GREEN "\033[0;32m" /* Green */
#define YELLOW "\033[0;33m" /* Yellow */
#define BLUE "\033[0;34m" /* Blue */
#define MAGENTA "\033[0;35m" /* Magenta */
#define CYAN "\033[0;36m" /* Cyan */
#define WHITE "\033[0;37m" /* White */
#define BACKBLACK "\033[0;40m" /* BACK Black */
#define BACKRED "\033[0;41m" /* BACK Red */
#define BACKGREEN "\033[0;42m" /* BACK Green */
#define BACKYELLOW "\033[0;43m" /* BACK Yellow */
#define BACKBLUE "\033[0;44m" /* BACK Blue */
#define BACKMAGENTA "\033[0;45m" /* BACK Magenta */
#define BACKCYAN "\033[0;46m" /* BACK Cyan */
#define BACKWHITE "\033[0;47m" /* BACK White */

#define GAMESIZE 10
int AisComputer, BisComputer;
int board[GAMESIZE][GAMESIZE];
bool used[GAMESIZE][GAMESIZE];
int scoreA = 0, scoreB = 0;
int jokerA, jokerB;
bool turn = 0; // 0 is Player A, 1 is Player B
int selectible;
char ErrorMessage[100];
char AutoPlayer();

bool isSelectible(int i, int j);

void SetErrorMessage(char * str) {
  strcpy(ErrorMessage, str);
}

void Initiate() {
  srand(time(NULL));
  for (int i = 0; i < GAMESIZE; i++)
    for (int j = 0; j < GAMESIZE; j++) {
      board[i][j] = rand() % 100 - 50;
      if (board[i][j] >= 0) board[i][j] += 1;
      used[i][j] = false;
    }
  selectible = GAMESIZE / 2;
  SetErrorMessage(" ");
  jokerA = jokerB = GAMESIZE / 3;

}

void DrawLine() {
  printf("       --------------------------------------------------------\n");
}

void PrintBoard() {
  printf("       %s", !turn ? YELLOW : "");
  for (int i = 0; i < GAMESIZE; i++)
    printf("     %c ", 65 + i);
  printf("\n"
    RESET);
  DrawLine();

  for (int i = 0; i < GAMESIZE; i++) {
    printf("    %s%c: "
      RESET, turn ? YELLOW : "", 65 + i);
    for (int j = 0; j < GAMESIZE; j++)
      if (isSelectible(i, j)) {
        if (!used[i][j])
          printf(YELLOW "[%5d]"
            RESET, board[i][j]);
        else
          printf("       ");
      }
    else {
      if (!used[i][j])
        printf("%6d ", board[i][j]);
      else
        printf("       ");
    }
    printf("\n");
  }
}

bool isEmpty(int rowcol) {
  if (!turn) {
    for (int i = 0; i < GAMESIZE; i++)
      if (!used[rowcol][i]) return false;
  } else {
    for (int i = 0; i < GAMESIZE; i++)
      if (!used[i][rowcol]) return false;
  }

  return true;
}

void PrintScores() {
  printf("\n %c [%c] Player A: %5d   ", !turn ? '*' : ' ', AisComputer ? 'C' : 'H', scoreA);
  for (int i = 0; i < jokerA; i++) printf("J");

  printf("\n %c [%c] Player B: %5d   ", turn ? '*' : ' ', BisComputer ? 'C' : 'H', scoreB);
  for (int i = 0; i < jokerB; i++) printf("J");
  printf("\n\n");
  DrawLine();
  printf(" *: Joker\n");
  printf(" X: Exit\n");

  printf(BACKRED "%s \n"
    RESET, ErrorMessage);
  printf("Select A row or column\n");
  DrawLine();
}

bool isSelectible(int i, int j) {
  if (!turn)
    return (i == selectible); // && (!used[i][j])
  else
    return (j == selectible); // && (!used[i][j]);
}

void changeSelectible() {
  int nonempty[GAMESIZE];
  int last = 0;

  for (int i = 0; i < GAMESIZE; i++)
    if (!isEmpty(i)) nonempty[last++] = i;

  selectible = nonempty[rand() % last];
}

void SelectPlayers() {
  char c;
  system("cls");
  printf("Select Player A: (H)uman or (C)omputer: ");
  c = toupper(getch());
  AisComputer = c == 'C';

  printf("\nSelect Player B: (H)uman or (C)omputer: ");
  c = toupper(getch());
  BisComputer = c == 'C';
}

void MainLoop() {
  char c = 0;
  while (c != 'X') {
    system("cls");
    PrintBoard();
    PrintScores();
    if (isEmpty(selectible)) {
      printf(YELLOW "Game Over\n");
      if (scoreA > scoreB)
        printf("Player A WIN!!");
      else if (scoreB > scoreA)
        printf("Player B WIN!!");
      else // ==
        printf("Draw Game!");
      printf(RESET);
      break;
    }

    if (!turn)
      c = AisComputer ? AutoPlayer() : toupper(getch());
    else
      c = BisComputer ? AutoPlayer() : toupper(getch());

    if (c == 'X') break;
    if (c == '*') {
      SetErrorMessage("");
      if (!turn) {
        if (jokerA == 0) {
          SetErrorMessage("You are out of Jokers");
          continue;
        }
        jokerA--;
      } else {
        if (jokerB == 0) {
          SetErrorMessage("You are out of Jokers");
          continue;
        }
        jokerB--;
      }
      turn = !turn;
      changeSelectible();
      continue;
    }
    if ((c < 65) || (c > 64 + GAMESIZE)) {
      SetErrorMessage("Select a row or coloumn in range");
      continue;
    }
    int idx = c - 65;
    //         for Player A								for Player B
    if ((!turn && used[selectible][idx]) || (turn && used[idx][selectible])) {
      SetErrorMessage("This cell is already used. Please select another cell");
      continue;
    }

    if (!turn) // Player A
    {
      // Add the score
      scoreA += board[selectible][idx];
      // Delete Number
      used[selectible][idx] = true;
    } else // Player B
    {
      // Add the score
      scoreB += board[idx][selectible];
      // Delete Number
      used[idx][selectible] = true;
    }
    // assign then new selection coloumn
    selectible = idx;
    SetErrorMessage("");
    turn = !turn;
  }
}

int main(int argc, char ** argv) {
  SelectPlayers();
  Initiate();
  MainLoop();
  return 0;
}

char AutoPlayer() {
  int maxId;
  //To store a the index of column or row of maximum score that enemy can choose.
  int tempMax[GAMESIZE] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100};
  //To store every column's or row's maximum score ( not including our row or column)
  //         So we can choose the less one for the enemy
  bool allMinus[GAMESIZE] = {true,true,true,true,true,true,true,true,true,true};
  //To store true bool if selected column or row is includes all minus numbers (not including our row or column)
  int choosenId;
  //To store Id that computer bot is going to use
  bool choosed = false;
  //to store if we choosed an Id

  for (int i = 0; i < GAMESIZE; i++) {   
  
  //PLAYER A START------------------
    if (!turn) { //Check which player's turn it is
    
        if (used[selectible][i]) continue;   
        //Check if cell is used before
        //    if it is used(true), continue the loop
        
      	bool biggerthan = false;
      	//To store if our current id (which is "i") plus 15 is bigger than the anything in that row or column. 
      	for(int j = 0; j < GAMESIZE; j++){ //Check enemy's lane (row or column)
    
      		if(used[j][i]) continue;
      		//if enemy's row or colum is already used, skip one cycle.
      		
      		if(board[j][i] > board[selectible][i] + 15) biggerthan = true;
      		//if enemy's possible choice is bigger than our current id(which is "i") plus 15, set biggerthan to true:
      		//Because opponent will earn minimum 15 more points then our choice
		
		    if (j == selectible) continue;
		    // check if j equals to selectible. if it is equal, then it is in our bot's lane. Enemy cant choose it. 
		    
		    if (board[j][i] > 0) allMinus[i] = false;
		    //if any value inside of the opponent's column, set current allminus to false;
		    
            if (board[j][i] > tempMax[i]) tempMax[i] = board[j][i];
            //Set current tempmax to biggest value in the current enemy column
		}
		
		if(!biggerthan && !choosed){
			//check if biggerthan is FALSE and check if it we haven't choosed any id
			choosenId = i;
            choosed = true;
			//set the choosen id and set choosed to true
		}
		else if(!biggerthan && choosed){
			if(board[selectible][i] - tempMax[i] > board[selectible][choosenId] - tempMax[choosenId]){
				choosenId = i;
			}
			
			//if it is already, calculate if current point difference is bigger than old one.
			//    if it is bigger, set new choosenid to current id
		}
    }
    //PLAYER A END-------------
    
    //PLAYER B START---------------
    //This part is almost same with the code part above. I just changed the column's and row's positions
    if (turn) {
        if (used[i][selectible]) continue;   
      	bool biggerthan = false;
      	    
      	for(int j = 0; j < GAMESIZE; j++){
      		
      		if(used[i][j]) continue;
      		if(board[i][j] > board[i][selectible] + 15) biggerthan = true;
		
		    if (j == selectible) continue;
		    if (board[i][j] > 0) allMinus[i] = false;
            if (board[i][j] > tempMax[i]) tempMax[i] = board[i][j];
		}
		
		if(!biggerthan && !choosed){
			choosenId = i;
            choosed = true;
		}
		else if(!biggerthan && choosed){
			if(board[i][selectible] - tempMax[i] > board[choosenId][selectible] - tempMax[choosenId]){
				choosenId = i;
			}
		}
    }
    //PLAYER B END----------------
    
  } //For Loop (i)

  int lastLow = 200;
  int lastLowId = 0;
  for (int i = 0; i < GAMESIZE; i++) {
    if (choosed) break;
    //If computer choose the best option in above, then break this loop.
    
    if (!turn && used[selectible][i]) continue;
    else if (turn && used[i][selectible]) continue;
    //To check if cell is used
    
    //PLAYER A START---------------
    if (!turn) {
    	//check which player's turn it is
      if (tempMax[i] > board[selectible][i]) {
      	//if enemy's current max is bigger than our selection,
      	
        if (abs(tempMax[i] - board[selectible][i]) < lastLow) {
        	//Calculate the difference and check if it is less then lastLow
        	
          lastLow = abs(tempMax[i] - board[selectible][i]);
          //set the new lastLow
          
          lastLowId = i;
          //and set the lastLowId
          
		  if(lastLow < 10 && board[selectible][i] > -10){
		  	//if lastLow is less then 10 and current selection is bigger then -10 then select this column or row
          	choosenId = lastLowId;
          	choosed = true;
		  }
        }
      }
    }
    //PLAYER A END-------
    
    //PLAYER B START----------------
        //This part is almost same with the code part above. I just changed the column's and row's positions
    if (turn) {
      if (tempMax[i] > board[i][selectible]) {
        if (abs(tempMax[i] - board[i][selectible]) < lastLow) {
          lastLow = abs(tempMax[i] - board[i][selectible]);
          lastLowId = i;
          if(lastLow < 10 && board[i][selectible] > -10){
          	choosenId = lastLowId;
          	choosed = true;
		  }
        }
      }
    }
    //PLAYER B END-------
    
    
  }
  

  if (!choosed) {
  	//if it is still not choosed,
  	
  	if(!turn)
  		if(jokerA > 0)
  		    return '*';	
	if(turn)
		if(jokerB > 0)
		    return '*';	
  	//Check turn and check if they have joker's left:
  	// if any joker left, use it
  	// else continue!
  	
  	
    for (int i = 0; i < GAMESIZE; i++) {
    	if(!turn && used[selectible][i]) continue;
    	if(turn && used[i][selectible]) continue;
    	//check if it is used
		if (tempMax[i] < tempMax[maxId] || (turn && used[maxId][selectible]) || (!turn && used[selectible][maxId])) maxId = i;
		//calculate and set maxId
		choosed = true;
    }
    if(choosed)
	    choosenId = maxId;
	    //set choosenId to maxId
  }
  return 'A' + choosenId;
//return the choosenId
}
