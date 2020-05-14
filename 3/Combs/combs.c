#include "combs.h"
#include <stdio.h>
#include <stdlib.h>

int comb(int* items, int** combs, int* cur_comb, int start, int n, int k, int maxk, int count)

int** get_combs(int* items, int k, int len){
	int nc = num_combs(len, k);
	int** combs = (int**)malloc(nc*sizeof(int*));
	int* cur_comb = (int*)malloc(k*sizeof(int));
	int i;
	for (i = 0; i < nc; i++){
		combs[i] = (int*)malloc(k * sizeof(int));
	}
	comb_helper(items, combs, cur_comb, 0, len, 0, k, 0);
	return combs;
}

int comb_helper(int* items, int** combs, int* cur_comb, int start, int n, int k, int maxk, int count){
	int i;
	if (k + 1 > maxk){
		for (i = 0; i < maxk; i++){
			combs[count][i] = cur_comb[i];
		}
		count++;
		return count;
	}
	for (i = start; i < n; i++){
		cur_comb[k] = items[i];
		count = comb_helper(items, combs, cur_comb, i+1, n, k+1, maxk, count);
	}
	return count;
}
