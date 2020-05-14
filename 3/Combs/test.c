#include <stdio.h>
#include <stdlib.h>
#include "combs.h"

void print_mat(int** mat, int num_rows, int num_cols);
void free_mat(int** mat, int num_rows, int num_cols);
int comb_helper(int* items, int** combs, int* cur_comb, int start, int n, int k, int maxk, int count);

int max(int a, int b){
  return a > b ? a : b;
}

int min(int a, int b){
  return a < b ? a : b;
}

int num_combs(int n, int k){
  int combs = 1;
  int i;

  for(i = n; i > max(k, n-k); i--){
    combs *= i;
  }

  for(i = 2; i <= min(k, n - k); i++){
    combs /= i;
  }

  return combs;

}

void print_mat(int** mat, int num_rows, int num_cols){
  int i,j;

  for(i = 0; i < num_rows; i++){
    for( j = 0; j < num_cols; j++){
      printf("%d ", mat[i][j]);
    }
    printf("\n");
  }
}

void free_mat(int** mat, int num_rows, int num_cols){
  int i;

  for(i = 0; i < num_rows; i++){
    free(mat[i]);
  }
  free(mat);
}





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
			//printf("combs = %d ", cur_comb[i]);
			combs[count][i] = cur_comb[i];
		}
		//printf("count = %d\n", count);
		count++;
    //printf("count++ = %d\n", count);
		return count;
	}
	for (i = start; i < n; i++){
		cur_comb[k] = items[i];
		count = comb_helper(items, combs, cur_comb, i+1, n, k+1, maxk, count);
	}
	return count;
}







int main(){
  int num_items;
  int* items;
  int i,k;
  int** combs;
  printf("How many items do you have: ");
  scanf("%d", &num_items);

  items = (int*) malloc(num_items * sizeof(int));

  printf("Enter your items: ");
  for(i = 0; i < num_items; i++){
    scanf("%d", &items[i]);
  }

  printf("Enter k: ");
  scanf("%d", &k);

  combs = get_combs(items, k, num_items);
  print_mat(combs,num_combs(num_items, k) ,k);
  free(items);
  free_mat(combs,num_combs(num_items, k), k);

  return 0;
}
