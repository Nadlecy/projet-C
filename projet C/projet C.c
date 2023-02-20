// projet C.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include "projet C.h"

#define M 6
#define N 6
char tab[M*N] = {'a'};



char initialize(char tab[M * N])
{
	int o;
	int i;
	int u; 
	char element = 'A';
	for (o = 0; o < N+1; o++)
	{
		printf("%d ", o);
	}
	printf("\n");
	for (i = 0; i < M; i++) 
	{
		printf("%d ",i+1);
		for (u = 0; u < N; u++) 
		{
			tab[i * M + u] = element;
			printf("%c ",tab[i*M+u]);
		}
		printf("\n");
	}
	return tab;
}

int main()
{
	char grid = initialize(tab);
	printf(tab);
	/*
	int i;
	int u;
	for (i = 0; i < M; i++) 
	{
		for (u = 0; u < N; u++) 
		{
			printf("[ ]");
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
