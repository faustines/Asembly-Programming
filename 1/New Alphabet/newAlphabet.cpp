/*
ECS 50 Fall 2018
Shreya Venkatesan
Faustine Yiu
*/

#include <iostream>
#include <deque>
#include <vector>
#include <string>

std::deque<unsigned int> convertNumToBin(unsigned int num, std::deque<unsigned int> binaryString){

	unsigned int newNum = num;
	while(newNum > 0) {
		if(newNum % 2 == 0){
		    binaryString.push_front(0);
		}
		else{
		    binaryString.push_front(1);
		}
		newNum  = newNum >> 1; //bit-shifting (keep dividing by 2 essentially)
	}
	while(binaryString.size() < 27){ //26 letters in the alphabet, so binaryString must be 0-25 or 26 bits
	    binaryString.push_front(0); //fill in remaining 0's b/c logical right shift
	}
  return binaryString; //binary representation of numbers entered in command line
}

std::string convertBinToLetter(std::deque<unsigned int> binaryString){
  std::string tempChar;
  int index = 0;
  int shifts = 0;
  int i = 1;
  //for loop to read through 0-25 to figure out the letter a-z (bits 1-26)
  while(!(binaryString.at(i) << 1)){
    i++;
    shifts++;
  }
  index = 25 - shifts;
  tempChar = 'a' + index;
  if(binaryString.at(0) == 1){ //looking at th 27th bit to see whether or not to capitalize
    tempChar = 'A' + index;
  }
  index = 0;
  return tempChar;
}

int main(int argc, char* argv[]) {

  std::deque<unsigned int> binaryString;
  std::string word = "";
  unsigned int number;
  std::string letter;

  for(int i = 1; i < argc; ++i){ //ignore 0 b/c of executable
    number = std::stoi(argv[i]); //convert the char into an integer
    binaryString = convertNumToBin(number, binaryString);
    letter = convertBinToLetter(binaryString); //takes in the resulting binary string from convertNumToBin
    word += letter;
    binaryString.clear();
  }
  std::cout << "You entered the word: " << word << std::endl;
  return 0;
}
