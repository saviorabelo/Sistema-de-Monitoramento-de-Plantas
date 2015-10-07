#include <stdio.h>
char Max (char v[], int n);
 
int main(void) {
	int n = 3;
	char v[n];
	v[0] = 'a';
	v[1] = 'd';
	v[2] = 'z';

	
	int x = Max (v, n);
	printf("\n%c\n", x);
	// your code goes here
	return 0;
}

/* A funcao abaixo recebe um inteiro n >= 1 e um vetor v e
 * devolve o valor de um elemento maxino de v[0..n-1]. */
 
 char Max (char v[], int n) {
 	int j, x = v[0];
 	for (j = 1; j < n; j++)
 		/* x e_ um elemento maximo de v[0..j-1] */
 		if (x < v[j]){
 		    x = v[j];
        }
 	return x;
 }
