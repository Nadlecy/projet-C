// projet C.c : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include "projet C.h"
#include <time.h>
#include <stdlib.h>

#define M 3
#define N 3

struct box 
{
	char * content;
	int isBomb;
	int nearbyBombs;
};

//struct box tab[M * N];

void initialize(struct box tab[M * N])
{
	int i, u; 
	struct box element = { "[ ]", 0, 0 };
	for (i = 0; i < M; i++) 
	{
		for (u = 0; u < N; u++) 
		{
			tab[i * M + u] = element;
			printf("%s", element.content);
		}
		printf("\n");
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
	int i;
	for (i = 0;i < M*N; i++)
	{
		if (tab[i].isBomb != 1)
		{
			//horizontals
			if (tab[i - 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			/*
			if (i % N < N - 1 && tab[i + 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//verticals
			if (i > N && tab[i-N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			if (i < (M*N-N+1) && tab[i+N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//diagonals up
			if (i > N && i % N > 0 && tab[i - 1 - N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			if (i > N && (i % N < N - 1) && tab[i + 1 - N].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			//diagonals down
			if (i < (M * N - N + 1) && i % N > 0 && tab[i + N - 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
			if (i < (M * N - N + 1) && i % N < N - 1 && tab[i + N + 1].isBomb)
			{
				tab[i].nearbyBombs++;
			}
*/
		}
		else
		{
			tab[i].nearbyBombs = 9;
		}
	}
}

int main()
{
	/*
	initialize(tab);
	bombPlacing(tab);
	bombRadar(tab);
	*/

	struct box test[9];
	struct box element = { "[ ]", 0, 0 };
	int p;
	for (p = 0; p < 9; p++)
	{
		test[p] = element;
	}
	test[5].isBomb = 1;
	test[6].isBomb = 1;
	test[8].isBomb = 1;

	bombRadar(test);

	int a, b;
	
	for (a = 0; a < M; a++) {
		for (b = 0; b < N; b++) 
		{
			printf("[%d]", test[a * b + b].nearbyBombs);
		}
		printf("\n");
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
