
#include <stdio.h>
int rec_strcmp(char* str1, char* str2);

int main(int argc, char** argv){
	char* str1 = argv[1];
	char* str2 = argv[2];
	int cmp_val = rec_strcmp(str1, str2);
	
	if(cmp_val < 0){
		printf("%s comes before %s\n", str1, str2);
	} else if(cmp_val > 0){
		printf("%s comes after %s\n", str1, str2);
	} else{
		printf("%s and %s are the same string\n", str1, str2);
	}
	
	return 0;
}
