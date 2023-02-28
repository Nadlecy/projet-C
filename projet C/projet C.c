// projet C.c : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include "projet C.h"
#include <time.h>
#include <stdlib.h>

/*
#define ROUGE "\x1b[31m"
#define VERT "\x1b[32m"
#define JAUNE "\x1b[33m"
#define BLEU "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
*/
#define COLOR_RESET "\x1b[0m"

struct box 
{
	char content;
	int isBomb;
	int nearbyBombs;
	char * color;
};

struct gameSettings 
{
	int width;
	int height;
	int bombTotal;
	int flags;
	int unopenedBoxes;
	int isGameDone;
};

//SETUP FUNCTIONS
//creates an empty grid, with the same dimensions as the player input
void initialize(struct box * tab, struct gameSettings* rules)
{
	struct box element = {' ', 0, 0};
	for (int i = 0; i < rules->height; i++)
	{
		for (int u = 0; u < rules->width; u++) 
		{
			tab[i * rules->height + u] = element;
		}
	}
}


//puts bombs on the field
void bombPlacing(struct box * tab, struct gameSettings* rules)
{
	int lower = 0;
	int upper = rules->width*rules->height-1;
	int i = 0;
	//implementing time variable, necessary for different generations
	time_t t1;
	srand((unsigned)time(&t1));

	while (i < rules->bombTotal) 
	{
		//selecting a random number between 1 and the length of the full list
		int num = (rand() % (rules->width * rules->height));

		//verifies if there is already a bomb at the place indicated by the called number
		if (tab[num].isBomb != 1)
		{
			//if not, it becomes one
			tab[num].isBomb = 1;
			i++;
		}
	}
}

//updates every box on the grid to tell how many bombs are nearby
void bombRadar(struct box * tab, struct gameSettings * rules)
{
	for (int i = 0;i < rules->width*rules->height; i++)
	{
		if (tab[i].isBomb != 1)
		{
			//horizontals 
			//(looking left)
			if (i>0 && i % rules->width != 0 && tab[i - 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			
			//(looking right)
			if (i % rules->width != rules->width -1 && tab[i + 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//verticals
			//(looking up)
			if (i > rules->width-1 && tab[i- rules->width].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(looking down)
			if (i < (rules->height * rules->width - rules->width) && tab[i+ rules->width].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//diagonals 
			//(upper left)
			if (i > rules->width-1 && i % rules->width > 0 && tab[i - 1 - rules->width].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(upper right)
			if (i > rules->width-1 && i % rules->width != rules->width - 1 && tab[i + 1 - rules->width].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(lower left)
			if (i < (rules->height * rules->width - rules->width) && i % rules->width > 0 && tab[i + rules->width - 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(lower right)
			if (i < (rules->height * rules->width - rules->width) && i % rules->width != rules->width - 1 && tab[i + rules->width + 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
		}
		else
		{
			tab[i].nearbyBombs = 9;
		}
		//setting boxes' colors depending on the number of nearby bombs
		switch (tab[i].nearbyBombs)
		{
			case 1:
				tab[i].color = "\x1b[34m"; //blue value
				break;
			case 2:
				tab[i].color = "\x1b[32m"; //green value
				break;
			case 3:
				tab[i].color = "\x1b[31m"; //red value
				break;
			case 4:
				tab[i].color = "\x1b[35m"; //magenta/purple value
				break;
			case 5:
				tab[i].color = "\x1b[36m"; //cyan value
				break;
			case 6:
				tab[i].color = "\x1b[36m"; //cyan value
				break;
			case 7:
				tab[i].color = "\x1b[36m"; //cyan value
				break;
			case 8:
				tab[i].color = "\x1b[33m"; //yellow value
				break;
			default :
				tab[i].color = "\x1b[0m"; //white value
		}
	}
}

//ACTION FUNCTIONS
//checks a box's contents and reveals it on the display
void dig(struct box * tab, int X, int Y, struct gameSettings* rules)
{
	// ckeck if there is a flag already placed or not on the tile
	if (X>0 && X< rules->height +1 && Y>0 && Y< rules->width +1 && tab[X - 1 + rules->width * (Y - 1)].content != 'P')
	{
		//if there is a bomb on the tile
		if (tab[X - 1 + rules->width * (Y - 1)].isBomb)
		{
			//you lose, so the game is done
			rules->isGameDone++;
			
		}
		//if the tile has not yet been revealed
		else if (tab[X - 1 + rules->width * (Y - 1)].content == ' ')
		{
			//if there is one (or more) bomb(s) near the tile 
			if ((tab[X - 1 + rules->width * (Y - 1)].nearbyBombs))
			{
				char newDisplay[2];
				//transforms the nearbyBombs pointer (int) into a string 
				sprintf_s(newDisplay, 2, "%d", tab[X - 1 + rules->width * (Y - 1)].nearbyBombs);
				tab[X - 1 + rules->width * (Y - 1)].content = newDisplay[0];
				//removes a number from the unopened boxes, because we just opened one
				rules->unopenedBoxes--;
			}
			//if there are no bombs around the current box, check every neighboring box 
			else
			{
				rules->unopenedBoxes--;
				tab[X - 1 + rules->width * (Y - 1)].content = '0';
				dig(tab, X - 1, Y, rules); //left
				dig(tab, X + 1, Y, rules); //right
				dig(tab, X - 1, Y + 1, rules); //down left
				dig(tab, X + 1, Y + 1, rules); //down right
				dig(tab, X - 1, Y - 1, rules); //up left
				dig(tab, X + 1, Y - 1, rules); //up left
				dig(tab, X, Y - 1, rules); //up
				dig(tab, X, Y + 1, rules); //down

			}
		}
	}
	//tell the player if the area they're trying to dig is flagged
	else if (X > 0 && X < rules->height + 1 && Y>0 && Y < rules->width + 1 && tab[X - 1 + rules->width * (Y - 1)].content == 'P')
	{
		printf("\nCan't dig a flagged area !");
	}
}

//puts or removes flags and question marks on boxes
void flag(struct box * tab, int X, int Y, struct gameSettings *rules)
{
	if (X > 0 && X < rules->width + 1 && Y>0 && Y < rules->height + 1)
	{
		//if the box is empty
		if (tab[X - 1 + rules->width * (Y - 1)].content == ' ')
		{
			//add a flag
			tab[X - 1 + rules->width * (Y - 1)].content = 'P';
			//decrease how many flags left the player has to put
			rules->flags--;
		}
		//if the box has a flag
		else if (tab[X - 1 + rules->width * (Y - 1)].content == 'P')
		{
			//replace the flag by a question mark
			tab[X - 1 + rules->width * (Y - 1)].content = '?';
			//increase how many flags left the player has to put 
			rules->flags++;
		}
		//if the box has a question mark
		else if (tab[X - 1 + rules->width * (Y - 1)].content == '?')
		{
			//empty the box
			tab[X - 1 + rules->width * (Y - 1)].content = ' ';
		}
	}
}

//gives a visual representation of the minefield
void displayGrid(struct box* tab, struct gameSettings* rules, int endDisplay)
{
	printf("\n");
	//puts a number indicator for each column
	for (int o = 0; o < rules->width + 1; o++)
	{
		//maintains the width of the shown string so that double digit numbers don't mess up the display
		if (o < 10)
		{
			printf(" %d ", o);
		}
		else
		{
			printf(" %d", o);
		}
	}
	//displaying each line
	for (int i = 0; i < rules->height; i++)
	{
		//starting with a number, same method as previously but only once per line
		if (i < 9)
		{
			printf("\n %d ", i + 1);
		}
		else
		{
			printf("\n %d", i + 1);
		}
		//displaying each box of the line
		for (int u = 0; u < rules->width; u++)
		{
			//if the game has ended and the box contains a mine, reveal the mine as an X
			if (tab[i * rules->height + u].isBomb && endDisplay)
			{
				tab[i * rules->height + u].content = 'X';
			}

			//shows the adequate content in the correspopnding color if it has been discovered via dig
			if (tab[i * rules->height + u].content != ' ' && tab[i * rules->height + u].content != 'P' && tab[i * rules->height + u].content != '?')
			{
				printf("%s""[%c]" COLOR_RESET, tab[i * rules->height + u].color, tab[i * rules->height + u].content);
			}
			//otherwise just show its content with no color
			else
			{
				printf("[%c]", tab[i * rules->height + u].content);
			}

		}
	}
}

//FUNCTIONS FOR OPTIMIZATION
//returns the number entered by the user after a custom query
int numQuery(char * numberIs, char * numberPurpose, int numberLimit)
{
	int Z = 0;
	while (Z <= 0 || Z > numberLimit)
	{
		printf("\nPlease choose a valid %s you would like to %s : ", numberIs, numberPurpose);
		scanf_s("%d", &Z);
		if (Z <= 0 || Z > numberLimit)
		{
			printf("\nThis %s isn't valid...", numberIs);
			while (getchar() != '\n');
		}
	}
	return Z;
}

//returns a character entered by the user after a query
char actionQuery()
{
	char ans = ' ';
	while (ans != 'D' && ans != 'd' && ans != 'F' && ans != 'f')
	{
		//asking the user what to do, dig or put a flag on the tile
		printf("\nWhat would you like to do here ? Dig or place a Flag (D/F): ");
		scanf_s("%c", &ans, 1);
		if (ans != 'D' && ans != 'd' && ans != 'F' && ans != 'f')
		{
			//if the answer is not valid, then we can't do anything, so we raise an error then
			printf("\nYou cannot do this here...");
			while (getchar() != '\n');
		}
	}
	return ans;
}

//the gameplay loop that will be repêated as long as the game is not over
void gamePlay(struct box *tab ,struct gameSettings* rules)
{
	//checks if any game-ending condition has been met (losing/winning) every turn
	while (rules->isGameDone == 0)
	{
		//Has the player opened every box except for mines ?
		if (rules->unopenedBoxes == 0)
		{
			rules->isGameDone = 1;
		}
		else
		{
			system("cls");
			//Show the current state of the minefield
			displayGrid(tab, rules, 0);
			printf("\nflags left:%d, unopened boxes:%d",rules->flags,rules->unopenedBoxes);
			//Take in the coordinates of the next box the player will act upon
			int X = numQuery("column", "play with",rules->width);
			int Y = numQuery("line", "play with", rules->height);

			//Emptying the stdin
			while (getchar() != '\n');
			//Selecting the next action to take
			char action = actionQuery();

			//Emptying the stdin again
			while (getchar() != '\n');
			
			//comitting the chosen action
			if (action == 'D' || action == 'd')
			{
				dig(tab, X, Y, rules);
			}
			else
			{
				flag(tab, X, Y, rules);
				
			}
		}
	}
}

void gameEnd(struct box* tab, struct gameSettings* rules)
{
	if (rules -> unopenedBoxes == 0)
	{
		//winning condition : all boxes have been opened
		printf("\n Congratulations, you win !");
	}
	else
	{
		//if you landed on a bomb before opening every box, well you lose 
		printf("\n You lose !");
	}
}

//asks the player if they want to do another game
int playAgain() {
	char ans = ' ';
	while (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n')
	{
		//asking the user if they want to play again
		printf("\nWould you like to play again ? (Y/N)\n");
		scanf_s("%c", &ans, 1);
		if (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n')
		{
			//if the answer is incorrect, then we raise an error
			printf("\nThat is not a valid answer.");
			while(getchar() != '\n');
		}
	}
	if (ans == 'Y' || ans == 'y')
	{
		//if they want to play again, then they play again
		return 1;
	}
	else if (ans == 'N' || ans == 'n')
	{
		//if not, the game ends here
		return 0;
	}

}

//main operating function, runs the whole program
int main()
{
	int playing = 1;
	while(playing)
	{
		//setting the grid size
		int xSize = numQuery("column amount", "set your grid width at", 256);
		int ySize = numQuery("line amount", "set your grid height at", 256);
		//setting the number of bombs in the grid
		int bombs = numQuery("amount of bombs", "put in your grid", xSize * ySize);

		struct gameSettings rules = { xSize, ySize, bombs, bombs, xSize * ySize - bombs, 0 };
		//beginning of game
		//
		//memory allocation for the grid called tab
		struct box* tab = (struct box*)malloc(sizeof(struct box) * rules.width * rules.height);
		initialize(tab, &rules);
		bombPlacing(tab, &rules);
		bombRadar(tab, &rules);

		//gameplay loop of interacting with the grid
		gamePlay(tab, &rules);
		//when game ends, tell if player won or lose
		system("cls");
		gameEnd(tab, &rules);
		displayGrid(tab, &rules, 1);

		//freeing the allocated memory of the grid
		free(tab);
		//ask player if they want to play again
		playing = playAgain();
	}

}