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
	int isGameDone;
};

struct box tab[M * N];
struct gameSettings rules = {10, 10, 10, 0};

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

void dig(struct box tab[M * N], int X, int Y, int endTrigger) 
{
	if (X>0 && X<M+1 && Y>0 && Y<N+1 && tab[X - 1 + N * (Y - 1)].content != 'P')
	{
		if (tab[X - 1 + N * (Y - 1)].isBomb)
		{
			endTrigger++;
		}
		else if (tab[X - 1 + N * (Y - 1)].content == ' ')
		{
			if ((tab[X - 1 + N * (Y - 1)].nearbyBombs))
			{
				char newDisplay[2];
				sprintf_s(newDisplay, 2, "%d", tab[X - 1 + N * (Y - 1)].nearbyBombs);
				tab[X - 1 + N * (Y - 1)].content = newDisplay[0];
			}
			else
			{
				tab[X - 1 + N * (Y - 1)].content = '0';
				dig(tab, X - 1, Y, endTrigger);
				dig(tab, X + 1, Y, endTrigger);
				dig(tab, X - 1, Y + 1, endTrigger);
				dig(tab, X + 1, Y + 1, endTrigger);
				dig(tab, X - 1, Y - 1, endTrigger);
				dig(tab, X + 1, Y - 1, endTrigger);
				dig(tab, X, Y - 1, endTrigger);
				dig(tab, X, Y + 1, endTrigger);

			}
		}
	}
}

void flag(struct box tab[M * N], int X, int Y, int nbBombsLeft, struct gameSettings *rules)
{
	if (X > 0 && X < M + 1 && Y>0 && Y < N + 1)
	{
		if (tab[X - 1 + N * (Y - 1)].content == ' ')
		{
			if (rules->flags) {
				tab[X - 1 + N * (Y - 1)].content = 'P';
				rules->flags--;
				if (tab[X - 1 + N * (Y - 1)].isBomb)
				{
					nbBombsLeft--;
				}
			}
		}
		else if (tab[X - 1 + N * (Y - 1)].content == 'P')
		{
			tab[X - 1 + N * (Y - 1)].content = '?';
			rules->flags++;
			if (tab[X - 1 + N * (Y - 1)].isBomb)
			{
				nbBombsLeft++;
			}
		}
		else if (tab[X - 1 + N * (Y - 1)].content == '?')
		{
			tab[X - 1 + N * (Y - 1)].content = ' ';
		}
	}
}


/*
void gamePlay()
{
	int X;
	int Y;
	char ans;
	printf("Please choose the column you would like to play in : ");
	scanf_s("%d", &Y);
	if (Y >= 0 || Y < M)
	{
		printf("It seems like you put the wrong coordinate... Please choose again.");
		while (Y >= 0 || Y < M)
		{
			printf("Please choose the column you would like to play in : ");
			scanf_s("%d", &Y);
		}
	}
	else 
	{
		printf("Please choose the column you would like to play in : ");
		scanf_s("%d", &X);
		if (X <= 0 || X > N)
		{
			printf("It seems like you put the wrong coordinate... Please choose again.");
			while (X <= 0 || X > N)
			{
				printf("Please choose the column you would like to play in : ");
				scanf_s("%d", &X);
			}
		}
		else
		{
			printf("What would you like to on this block : Dig or Place a Flag ? (answer D/d or F/f) : ");
			scanf_s("%c",&ans, ans);
			if (ans != 'D' || ans != 'd' || ans != 'F' || ans != 'f')
			{
				printf("It looks like your answer isn't something we were expecting... please select again.");
				while (ans != 'D' || ans != 'd' || ans != 'F' || ans != 'f')
				{
					printf("What would you like to on this block : Dig or Place a Flag ? (answer D or F) : ");
					scanf_s("%c", &ans, ans);
				}
			}
			else
			{
				if (ans == 'D' || ans == 'd')
				{
					dig(tab, X, Y, rules.isGameDone);
				}
				else
				{
					flag(tab, X, Y, rules.bombsLeft, rules.flags);
				}
			}
		}
	}
}
*/
int main()
{
	
	initialize(tab);
	bombPlacing(tab, rules.bombTotal);
	bombRadar(tab);
	displayGrid(tab);
	flag(tab, 1, 1, rules.bombsLeft, &rules);
	displayGrid(tab);
	printf("%d", rules.flags);
	flag(tab, 1, 1, rules.bombsLeft, &rules);
	displayGrid(tab);
	printf("%d", rules.flags);


	/*
	struct box test[9];
	struct box element = { "[ ]", 0, 0 };
	int p;
	for (p = 0; p < 9; p++)
	{
		test[p] = element;
	}
	test[3].isBomb = 1;
	test[1].isBomb = 1;
	test[8].isBomb = 1;

	bombRadar(test);

	int a, b;
	
	for (a = 0; a < M; a++) {
		for (b = 0; b < N; b++) 
		{
			printf("[%d]", test[a * M + b].nearbyBombs);
		}
		printf("\n");
	}
	*/
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
