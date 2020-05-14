#include <iostream>
#include <fstream>
#include <vector>

int calcIndex (std::vector<int> v, int x, int y, int N){
  if ((y < x) && (x > 0)){
    return 0;
  }else{
    int index = N * x - (x-1)*x/2 + (y-x);
    return v.at(index);
  }
}

int main(int argc, char** argv){

  std::vector<int>matrixA;
  std::vector<int>matrixB;
  std::vector<int> matrixC;
  std::vector<int> zero;
  std::ifstream A;
  std::ifstream B;
  int index = 0;
  int N = 0;
  int temp = 0;
  int zeroCount = 0;
  int limit = 1;
  int zeroTimes = -1;

  A.open(argv[1]);
  if(!A){std::cout << "Unable to open file" << std::endl; return 0;}
  A >> N;
  while(A >> temp){
    matrixA.push_back(temp);
  }
  A.close();

  B.open(argv[2]);
  if(!B){std::cout << "Unable to open file" << std::endl; return 0;}
  B >> N;
  while(B >> temp){
    matrixB.push_back(temp);
  }
  B.close();

  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      for(int k = 0; k < N; k++) {
        index += calcIndex(matrixA,i,k,N) * calcIndex(matrixB,k,j,N);
      }

      if(index == 0){
        zeroTimes++;
        if(zeroCount < limit){
          zero.push_back(index);
          zeroCount++;
        }else{
          matrixC.push_back(index);
          zeroCount++;
        }
    }else{
      matrixC.push_back(index);
      zeroCount = 0;
    }
    if (zeroTimes == limit) {
      limit++;
      zeroTimes = 0;
    }
    index = 0;
  }
}

  for (int i = 0; i < matrixC.size(); i++) {
    std::cout << matrixC.at(i) << " ";
  }

}
