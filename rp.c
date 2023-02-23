/*
 * rp.c
 *
 *      Author: Michele
 */

#include "rp.h"
#include <stdlib.h>
#include <time.h>

int Partition(array a, int p, int r) {
	int x = a[r];
	int i = p-1;
	for(int j = p; j < r; j++) {
		if(a[j] <= x) {
			i++;
			Swap(&a[i], &a[j]);
		}
	}
	Swap(&a[i+1], &a[r]);
	return i+1;
}

void Swap(int* x, int* y) {
	int t = *x;
	*x = *y;
	*y = t;
}

int RandomizedPartition(array a, int p, int r) {
	int i = Random(p, r);
	Swap(&a[r], &a[i]);
	return Partition(a, p, r);
}

int Random(int p, int r) {
	srand(time(0));
	return ((rand() % (r-p+1)) + p);
}

int RandomizedSelect(array a, int p, int r, int i) {
	if(p == r)
		return a[p];
	int q = RandomizedPartition(a, p, r);
	//int q = Partition(a, p, r); //per il worst case
	int k = q-p+1;
	if(i == k)
		return a[q];
	else {
		if(i < k)
			return RandomizedSelect(a, p, q-1, i);
		else
			return RandomizedSelect(a, q+1, r, i-k);
	}
}
