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

struct box tab[M * N];

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

void bombPlacing(struct box tab[M * N])
{
	int bombRatio = 10;
	//if bombRatio <= 0 || bombRatio >= 100;
	int lower = 1;
	int upper = M*N;
	int i;
	time_t t1;
	srand((unsigned)time(&t1));;
	for (i = 0; i < bombRatio; i++)
	{
		int num = (rand() %(upper - lower + 1)) + lower;
		tab[num].isBomb = 1;
	}
}

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
	for (int o = 0; o < M + 1; o++) 
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

void touchBox(struct box tab[M * N], int X, int Y) 
{
	if (tab[X-1 + N*(Y-1)].isBomb)
	{
		//Loss
	}
	else
	{
		if ((tab[X - 1 + N * (Y - 1)].nearbyBombs))
		{
			char newDisplay[2];
			sprintf_s(newDisplay, 2, "%d", tab[X - 1 + N * (Y - 1)].nearbyBombs);
			tab[X - 1 + N * (Y - 1)].content = newDisplay[0];
		}
		else
		{
			tab[X - 1 + N * (Y - 1)].content = "0";
			touchBox(tab, X - 1, Y);
			touchBox(tab, X + 1, Y);
			touchBox(tab, X - 1, Y + 1);
			touchBox(tab, X + 1, Y + 1);
			touchBox(tab, X - 1, Y - 1);
			touchBox(tab, X + 1, Y - 1);
			touchBox(tab, X , Y - 1);
			touchBox(tab, X , Y + 1);
			
		}

	}
}

int main()
{
	
	initialize(tab);
	bombPlacing(tab);
	bombRadar(tab);
	displayGrid(tab);

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
