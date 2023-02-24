// projet C.c : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include "projet C.h"
#include <time.h>
#include <stdlib.h>

#define M 10
#define N 10

struct box 
{
	char content;
	int isBomb;
	int nearbyBombs;
};

struct gameSettings 
{
	int bombTotal;
	int bombsLeft;
	int flags;
	int unopenedBoxes;
	int isGameDone;
};

struct box tab[M * N];
struct gameSettings rules = {10, 10, 10, M*N - 10, 0};

//creates an empty grid, dimensions M Lines * N Columns
void initialize(struct box tab[M * N])
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

//puts bombs on the field
void bombPlacing(struct box tab[M * N], int bombNumber)
{
	int lower = 1;
	int upper = M*N;
	int i;
	time_t t1;
	srand((unsigned)time(&t1));;
	for (i = 0; i < bombNumber; i++)
	{
		int num = (rand() %(upper - lower + 1)) + lower;
		tab[num].isBomb = 1;
	}
}

//updates every box on the grid to tell how many bombs are nearby
void bombRadar(struct box tab[M * N])
{
	for (int i = 0;i < M*N; i++)
	{
		if (tab[i].isBomb != 1)
		{
			//horizontals 
			//(looking left)
			if (i>0 && i % M != 0 && tab[i - 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			
			//(looking right)
			if (i % N != N-1 && tab[i + 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//verticals
			//(looking up)
			if (i > N && tab[i-N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(looking down)
			if (i < (M*N-N) && tab[i+N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//diagonals 
			//(upper left)
			if (i > N && i % N > 0 && tab[i - 1 - N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(upper right)
			if (i > N && i % N != N - 1 && tab[i + 1 - N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(lower left)
			if (i < (M * N - N) && i % N > 0 && tab[i + N - 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//(lower right)
			if (i < (M * N - N) && i % N != N - 1 && tab[i + N + 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
		}
		else
		{
			tab[i].nearbyBombs = 9;
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
			printf("[%c]", tab[i * M + u].content);
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
			if (tab[X - 1 + N * (Y - 1)].isBomb)
			{
				rules->bombsLeft--;
			}
		}
		else if (tab[X - 1 + N * (Y - 1)].content == 'P')
		{
			tab[X - 1 + N * (Y - 1)].content = '?';
			rules->flags++;
			if (tab[X - 1 + N * (Y - 1)].isBomb)
			{
				rules->bombsLeft++;
			}
		}
		else if (tab[X - 1 + N * (Y - 1)].content == '?')
		{
			tab[X - 1 + N * (Y - 1)].content = ' ';
		}
	}
}

//FUNCTIONS FOR OPTIMIZATION


int positionQueries(char * direction, int directionLimit)
{
	int Z = 0;
	while (Z <= 0 || Z > directionLimit)
	{
		printf("\nPlease choose a valid %s you would like to play in : ", direction);
		scanf_s("%d", &Z);
		if (Z <= 0 || Z > directionLimit)
		{
			printf("\nThis %s does not exist...", direction);
		}
	}
	return Z;
}

char actionQueries()
{
	char ans = ' ';
	while (ans != 'D' && ans != 'd' && ans != 'F' && ans != 'f')
	{
		printf("\nWhat would you like to do here ? Dig or place a Flag (D/F): ");
		scanf_s("%c", &ans, 1);
		if (ans != 'D' && ans != 'd' && ans != 'F' && ans != 'f')
		{
			printf("\nYou cannot do this here...");
			getchar();
		}
	}
	return ans;
}

void gamePlay(struct gameSettings* rules)
{
	//checks if any game-ending condition has been met (losing/winning)
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
			//Take in the coordinates of the next box the player will act upon
			int X = positionQueries("column", N);
			int Y = positionQueries("line", M);

			//Emptying the stdin
			while (getchar() != '\n');
			//Selecting the next action to take
			char action = actionQueries();

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

			/*
			if (ans != 'D' || ans != 'd' || ans != 'F' || ans != 'f')
			{
				printf("It looks like your answer isn't something we were expecting... please select again.");
				while (ans != 'D' || ans != 'd' || ans != 'F' || ans != 'f')
				{
					printf("\nWhat would you like to on this block : Dig or Place a Flag ? (answer D or F) : ");
					scanf_s("%c", &ans, ans);
				}
			}
			else
			{
				if (ans == 'D' || ans == 'd')
				{
					dig(tab, X, Y, rules);
				}
				else
				{
					flag(tab, X, Y, rules);
				}
			}*/
		}
	}
}

int main()
{
	int playing = 1;
	while(playing)
	{
		//beginning of game
		initialize(tab);
		bombPlacing(tab, rules.bombTotal);
		bombRadar(tab);

		//gameplay loop of interacting with the grid
		gamePlay(&rules);

		if (rules.unopenedBoxes == 0)
		{
			printf("\n Congratulations, you win !");
		}
		else
		{
			printf("\n You lose !");
		}

		//demander au joueur s'il veut rejouer
	}
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
