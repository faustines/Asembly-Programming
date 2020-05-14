/*
ECS 50 Fall 2018
Shreya Venkatesan
Faustine Yiu
*/

#include <iostream>
#include <string>
#include <cmath> //pow
#include <locale> //isdigit

unsigned int convertToBaseTen(int currBase, int newBase, std::string number){

    unsigned int convertedNum = 0;
    int temp;
    unsigned int getVal = 0;
    for(unsigned int i = 0; i < number.length(); ++i){
       char val = number[(number.length() - i - 1)];
        if(isdigit(val)){ //0-9
            temp = (val - '0');
        }
        else{ //A-Z
            temp = (val - 'A') + 10; //A = 10
        }
        getVal = temp * pow(currBase, i);
        convertedNum += getVal;
    }
    return convertedNum;
}

std::string changeBase(int currBase, int newBase, std::string number){

    std::string newNum;
    char temp;
    unsigned int num = convertToBaseTen(currBase, newBase, number);
    while(num > 0){
        int left = num % newBase;
        if(left < 10){ //0-9
            temp = left + '0';
        }
        else{ //A-Z
            temp = (left - 10) + 'A'; //A = 10
        }
        newNum = temp + newNum;
        num = num / newBase;
    }
    return newNum;
}

int main(){

  int currBase = 0;
  int newBase = 0;
  //input may or may not be a number so need to convert from string
  std::string number;

  std::cout << "Please enter the number's base: ";
  std::cin >> currBase;
  std::cout << "Please enter the number: ";
  std::cin >> number;
  std::cout << "Please enter the new base: ";
  std::cin >> newBase;

  std::string conversion = changeBase(currBase, newBase, number);
  std::cout << number << " base " << currBase << " is " << conversion << " base " << newBase << std::endl;

  return 0;
}
