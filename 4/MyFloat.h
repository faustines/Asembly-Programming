#ifndef MY_FLOAT_H
  #define MY_FLOAT_H

  #include <iostream>
  using namespace std;

  class MyFloat{
    public:
      //constructors

      MyFloat();
      MyFloat(float f);
      MyFloat(const MyFloat & rhs);
      virtual ~MyFloat() {};



      //output
      friend ostream& operator<<(std::ostream& strm, const MyFloat& f);

      //equals
      bool operator==(const MyFloat& rhs) const;

      //addition
      MyFloat operator+(const MyFloat& rhs) const;

      //subtraction
      MyFloat operator-(const MyFloat& rhs) const;


      static int timeToCarry(const unsigned int num1, const unsigned int num2);
      const int Greater(const MyFloat& myFloat) const;
      const MyFloat isNegAndSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const;
      const MyFloat justSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const;
      const MyFloat notNegAndSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const;
      const MyFloat notNegAndDiffSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const;

    private:
      unsigned int sign;
      unsigned int exponent;
      unsigned int mantissa;

      void unpackFloat(float f);
      float packFloat() const;

      const int timeToBorrow(unsigned int Mant, const int diffExpExp) const;
      const MyFloat notNegAndSameSignCarry(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const;
      const MyFloat isNegAndSameSignCarry(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const;
      const MyFloat notNegAndDiffSignBorrow(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const;
      const MyFloat isNegAndDiffSignBorrow(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const;



  }; // myFloat


#endif
