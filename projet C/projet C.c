// projet C.c : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include "projet C.h"
#include <time.h>
#include <stdlib.h>

#define M 10
#define N 10
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

struct box tab[M * N];

struct gameSettings 
{
	int width;
	int height;
	int bombTotal;
	int flags;
	int unopenedBoxes;
	int isGameDone;
};

/*
//creates an empty grid, with the same dimensions as the player input
void initialize(struct box tab[M * N], struct gameSettings* rules)
{
	struct box element = {' ', 0, 0 };
	for (int i = 0; i < M; i++) 
	{
		for (int u = 0; u < N; u++) 
		{
			tab[i * M + u] = element;
		}
	}
}
*/

//puts bombs on the field
void bombPlacing(struct box * tab, struct gameSettings* rules)
{
	int lower = 1;
	int upper = rules->width*rules->height;
	int i;
	time_t t1;
	srand((unsigned)time(&t1));;
	for (i = 0; i < rules->bombTotal; i++)
	{
		int num = (rand() %(upper - lower + 1)) + lower;
		tab[num].isBomb = 1;
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
			if (i > rules->width && tab[i- rules->width].isBomb)
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
			if (i > rules->width && i % rules->width > 0 && tab[i - 1 - rules->width].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(upper right)
			if (i > rules->width && i % rules->width != rules->width - 1 && tab[i + 1 - rules->width].isBomb)
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

void displayGrid(struct box tab[M * N])
{
	printf("\n");
	for (int o = 0; o < N + 1; o++) 
	{
		if (o < 10)
		{
			printf(" %d ", o);
		}
		else
		{
			printf(" %d", o);
		}
	}
	for (int i = 0; i < M; i++)
	{
		if (i < 9)
		{
			printf("\n %d ", i + 1);
		}
		else
		{
			printf("\n %d", i + 1);
		}
		for (int u = 0; u < N; u++)
		{
			if (tab[i * M + u].content != ' ')
			{
				printf("%s""[%c]" COLOR_RESET, tab[i * M + u].color, tab[i * M + u].content);
			}
			else
			{
				printf("[%c]", tab[i * M + u].content);
			}
		}
	}
}

void dig(struct box tab[M * N], int X, int Y, struct gameSettings* rules)
{
	if (X>0 && X<M+1 && Y>0 && Y<N+1 && tab[X - 1 + N * (Y - 1)].content != 'P')
	{
		if (tab[X - 1 + N * (Y - 1)].isBomb)
		{
			rules->isGameDone++;
			
		}
		else if (tab[X - 1 + N * (Y - 1)].content == ' ')
		{
			if ((tab[X - 1 + N * (Y - 1)].nearbyBombs))
			{
				char newDisplay[2];
				sprintf_s(newDisplay, 2, "%d", tab[X - 1 + N * (Y - 1)].nearbyBombs);
				tab[X - 1 + N * (Y - 1)].content = newDisplay[0];
				rules->unopenedBoxes--;
			}
			else
			{
				rules->unopenedBoxes--;
				tab[X - 1 + N * (Y - 1)].content = '0';
				dig(tab, X - 1, Y, rules);
				dig(tab, X + 1, Y, rules);
				dig(tab, X - 1, Y + 1, rules);
				dig(tab, X + 1, Y + 1, rules);
				dig(tab, X - 1, Y - 1, rules);
				dig(tab, X + 1, Y - 1, rules);
				dig(tab, X, Y - 1, rules);
				dig(tab, X, Y + 1, rules);

			}
		}
	}
}

void flag(struct box tab[M * N], int X, int Y, struct gameSettings *rules)
{
	if (X > 0 && X < M + 1 && Y>0 && Y < N + 1)
	{
		if (tab[X - 1 + N * (Y - 1)].content == ' ')
		{
			tab[X - 1 + N * (Y - 1)].content = 'P';
			rules->flags--;
		}
		else if (tab[X - 1 + N * (Y - 1)].content == 'P')
		{
			tab[X - 1 + N * (Y - 1)].content = '?';
			rules->flags++;
		}
		else if (tab[X - 1 + N * (Y - 1)].content == '?')
		{
			tab[X - 1 + N * (Y - 1)].content = ' ';
		}
	}
}

//FUNCTIONS FOR OPTIMIZATION

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

char actionQuery()
{
	char ans = ' ';
	while (ans != 'D' && ans != 'd' && ans != 'F' && ans != 'f')
	{
		printf("\nWhat would you like to do here ? Dig or place a Flag (D/F): ");
		scanf_s("%c", &ans, 1);
		if (ans != 'D' && ans != 'd' && ans != 'F' && ans != 'f')
		{
			printf("\nYou cannot do this here...");
			while (getchar() != '\n');
		}
	}
	return ans;
}

void gamePlay(struct gameSettings* rules)
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
			//Show the current state of the minefield
			displayGrid(tab);
			printf("flags left:%d, unopened boxes:%d",rules->flags,rules->unopenedBoxes);
			//Take in the coordinates of the next box the player will act upon
			int X = numQuery("column", "play with",N);
			int Y = numQuery("line", "play with", M);

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

void gameEnd(struct gameSettings* rules)
{
	if (rules -> unopenedBoxes == 0)
	{
		printf("\n Congratulations, you win !");
	}
	else
	{
		printf("\n You lose !");
	}
}

int playAgain() {
	char ans = ' ';
	while (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n')
	{
		printf("\nWould you like to play again ? (Y/N)\n");
		scanf_s("%c", &ans, 1);
		if (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n')
		{
			printf("\nThat is not a valid answer.");
			while(getchar() != '\n');
		}
	}
	if (ans == 'Y' || ans == 'y')
	{
		return 1;
	}
	else if (ans == 'N' || ans == 'n')
	{
		return 0;
	}

}

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
		//initialize(tab, &rules);
		//memory allocation for the grid called tab
		struct box* tab = (struct box*)malloc(sizeof(struct box) * rules.width * rules.height);
		bombPlacing(tab, &rules);
		bombRadar(tab);

		//gameplay loop of interacting with the grid
		gamePlay(&rules);
		//when game ends, tell if player won or lost
		gameEnd(&rules);

		//freeing the allocated memory of the grid
		free(tab);
		//ask player if they want to play again
		playing = playAgain();
	}
}