/*
ECS 50 Fall 2018
Shreya Venkatesan
Faustine Yiu
*/

#include <stdio.h>
#include <stdlib.h>

void division(unsigned long dividend, unsigned long divisor, unsigned int* quotient, unsigned int* R){

    //O(1) Runtime
    for(int i = 0; i < 32; i++){ //max 32 iterations (bit 0 to bit 31)
		*R <<= 1; //multiply by 2
		if (dividend & 0x80000000){ //0x80000000 = 2^31
			(*R)++;
		}
		if (*R >= divisor){ //remainder is greater than/equal to divisor
			*quotient |= 1 << (31-i); //set LSB (least significant bit) to 1
			*R -= divisor; //update the remainder value
		}
		dividend <<= 1; //multiply by 2
    }
    return;
}

int main(int argc, char* argv[]) {

    unsigned long dividend = atoll(argv[1]);
	unsigned long divisor = atoll(argv[2]);
    unsigned int quotient = 0;
    unsigned int R = 0;

    division(dividend, divisor, &quotient, &R);
    printf("%lu / %lu = %d R %d\n", dividend, divisor, quotient, R);
    return 0;
}
