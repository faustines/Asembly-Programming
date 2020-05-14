/*
ECS 50 Fall 2018
Shreya Venkatesan
Faustine Yiu
*/

#include <iostream>
#include <string>

std::string convertToBinary(std::string binaryString, unsigned int float_int){
    while(float_int != 0) {
		if(float_int % 2 == 0){
		    binaryString = '0' + binaryString;
		}
		else{
		    binaryString = '1' + binaryString;
		}
		float_int  = float_int >> 1; //bit-shifting (keep dividing by 2 essentially)
	}
    return binaryString;
}

void isNegative(unsigned int float_int){
    if(float_int >> 31){ //the 31st bit is 1
        std::cout << "-"; //1 = negative, 0 = positive
    }
}

unsigned int findMantissa(unsigned int float_int){
    unsigned int mantissa = float_int << 9; //bits 0-22 is mantissa
    //extract the mantissa out of the 0's filled in from left-shifting
    while(!(mantissa%2)){
        mantissa = mantissa >> 1;
    }
    return mantissa;
}

int findExp(unsigned int float_int){
    //bits 23-30 gives you the exponent
    return ((float_int << 1) >> 24)-127; //left shift by 1, right shift by 24
}

int main(){

  float input = 0.0;
  std::cout << "Please enter a float: ";
  std::cin >> input;
  std::string binaryString = "";
  unsigned int float_int = *((unsigned int*)&input);

  isNegative(float_int);
  unsigned int mantissa = findMantissa(float_int);
  binaryString = convertToBinary(binaryString, mantissa);
  int exponent = findExp(float_int);
  std::cout << "1." << binaryString << "E" << exponent << std::endl;

  return 0;
}
