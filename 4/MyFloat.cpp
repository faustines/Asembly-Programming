#include "MyFloat.h"
#include <iostream>
using namespace std;

MyFloat::MyFloat()
{
  sign = 0;
  exponent = 0;
  mantissa = 0;

} // MyFloat()

MyFloat::MyFloat(float f)
{
  unpackFloat(f);
} // MyFloat(float f)

MyFloat::MyFloat(const MyFloat &rhs)
{
	sign = rhs.sign;
	exponent = rhs.exponent;
	mantissa = rhs.mantissa;
} // MyFloat(const MyFloat &rhs)

ostream& operator<<(std::ostream &strm, const MyFloat &f)
{
	strm << f.packFloat();
	return strm;

} // operator<<(std::ostream &strm, const MyFloat &f)


bool MyFloat::operator==(const MyFloat& rhs) const
{
  if ((exponent == rhs.exponent && sign == rhs.sign) && (mantissa == rhs.mantissa))
    return true;
  else
    return false;
} // operator==(const MyFloat& rhs) const

const int MyFloat::Greater(const MyFloat& myFloat) const
{
  // To determine larger absolute value
  MyFloat float1(*this);
  MyFloat float2(myFloat);
  float1.sign = 0;
  float2.sign = 0;

  if (float1.packFloat() < float2.packFloat())
    return myFloat.sign;

  if (float1.packFloat() > float2.packFloat())
    return sign;

  return 0; // equal
}  // Greater(const MyFloat& myFloat) const

int MyFloat::timeToCarry(const unsigned int number1, const unsigned int number2)
{
  int flag = 1; // Flag is set to true
 __asm__
 (
  // code section
  // ebx is number1
  // ecx is number2
  // edx is flag
 "addl %%ebx, %%ecx;"
 "jc jumpCarry;"
 "movl $0, %%edx;" // edx --> flag
 "jmp done;"

 "jumpCarry:"
    "movl $1, %%edx;"

 "done:"
    "addl %%ebx, %%ecx":

  // outputs
 "=d" (flag):
  // inputs
 "b" (number1), "c" (number2):

  "cc"
  );
  return flag; // returns either true or false.

} // timeToCarry(const unsigned int number1, const unsigned int number2)

void MyFloat::unpackFloat(float f)
{
__asm__
(
"clc;"
// code section
// edx is f
// eax is mantissa
// ebx is sign
// ecx is exponent

"movl %%edx, %%eax;" // eax now contains f
"shll $9, %%eax;" // f >> 9
"shrl $9, %%eax;" // f << 9 --> pointed by eax.

"movl %%edx, %%ecx;" // ecx contains f
"shll $1, %%ecx;" // f << 1
"shrl $1, %%ecx;" // f >> 1
"shrl $23, %%ecx;" // f >> 23

"movl %%edx, %%ebx;" // ebx now contains f
"shrl $31, %%ebx": // 31st bit contains the sign.

// outputs section
"=b" (sign), "=c" (exponent), "=a" (mantissa):

// inputs section
"d" (f):

"cc"
);
} //unpackFloat(float f)

float MyFloat::packFloat() const
{
  //returns the floating point representation
  float f;
  __asm__
  (
  // code
  // ebx is sign
  // eax is exponent
  // edx is mantissa
  // ecx is float
  "movl $0, %%ecx;" // float = 0;
  "shll $31, %%ebx;" // sign << 31;
  "shll $23, %%eax;" // exponent << 23;

  "or %%eax, %%ecx;" // float = float | exponent;
  "or %%ebx, %%ecx;" // float = float | sign;
  "or %%edx, %%ecx": // float = float | mantissa;

  // output sectuon
  "=c" (f):

  // input section
  "b" (sign), "a" (exponent), "d" (mantissa):
  "cc"
  );
  return f;
} //packFloat() const

const int MyFloat::timeToBorrow(unsigned int mantissa, const int diffExp) const
{
  mantissa = mantissa >> (diffExp - 1);
  if ((mantissa & (0x1 << 0)) == 0)
    return 0; // Don't borrow

  else
    return 1; // Borrow
} // timeToBorrow(unsigned int mantissa, const int diffExp) const

MyFloat MyFloat::operator+(const MyFloat& rhs) const
{
  MyFloat add1(*this);
  MyFloat add2(rhs);
  MyFloat *a, *b;
  a = &add1;
  b = &add2;

  int greater = Greater(rhs), getMSB = 0;
  int diffExp = exponent - rhs.exponent;
  bool isNeg = false, isSameSign = false;

  if ((mantissa == rhs.mantissa) && (exponent == rhs.exponent) && (sign != rhs.sign))
    return 0;

  if (diffExp < 0)
  {
    isNeg = true;
    diffExp = -(diffExp);
  }

  if (sign == rhs.sign)
    isSameSign = true;

  add1.mantissa = add1.mantissa | (0x1 << 23); // 24th bit is set to 1 -> add1
  add2.mantissa = add2.mantissa | (0x1 << 23); // 24th bit is set to 1 -> add2

  if (diffExp <= 0x8)
  {
  	if ((isNeg && isSameSign) == 1)
      return isNegAndSameSign(a, b, getMSB, diffExp);

    if ((!(isNeg || isSameSign)) == 1)
  	  return notNegAndDiffSign(a, b, getMSB, diffExp, greater);

    if ((!(!isNeg || isSameSign)) == 1)
  	  return notNegAndSameSign(a, b, getMSB, diffExp, greater);

  	if (isSameSign == true)
      return justSameSign(a, b, getMSB, diffExp);
   } // if diffExperence is 8

  if (diffExp > 0x8)
  {

    if ((!(isNeg || !isSameSign)) == 1)
      return notNegAndSameSignCarry(a, b, getMSB, diffExp);

    if ((!(!isNeg || !isSameSign)) == 1)
      return isNegAndSameSignCarry(a, b, getMSB, diffExp);

    if ((!(isNeg || isSameSign)) == 1)
      return notNegAndDiffSignBorrow(a, b, getMSB, diffExp, greater);

    if ((!(!isNeg || isSameSign)) == 1)
      return isNegAndDiffSignBorrow(a, b, getMSB, diffExp, greater);

  } // if difference greater than 8
  return 0;
} // operator+(const MyFloat& rhs) const

MyFloat MyFloat::operator-(const MyFloat& rhs) const
{
  MyFloat temprhs(rhs);
  MyFloat tempthis(*this);
  if (temprhs.sign == 0)
    temprhs.sign = 1;

  else if (temprhs.sign == 1)
    temprhs.sign = 0;

  tempthis = tempthis.operator+(temprhs);
	return tempthis;
} // operator-(const MyFloat& rhs) const

const MyFloat MyFloat::isNegAndSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const
{
  unsigned int Exp2 = add2->exponent;
  unsigned int Mant = add1->mantissa,  Mant2 = add2->mantissa;

  add2->mantissa = Mant2 << diffExp;
  add2->exponent = Exp2 - diffExp;

  Mant2 = add2->mantissa;
  Mant = add1->mantissa;

  if (timeToCarry(add1->mantissa,add2->mantissa) == 1)
  {
    add1->mantissa = Mant >> 1; // Right shift by 1
    add2->mantissa = Mant2 >> 1; // Right shift Mant2 by 1
    add2->exponent++;
  } // if flag is true.

  add2->mantissa = add1->mantissa + add2->mantissa;

  int i = 31;
  Mant2 = add2->mantissa;
  do
  {
    if (!(Mant2 & ( 0x1 << i)))
      i--;
    else
    {
      getMSB = i - 23;
      i--;
      break;
    }

  } while(i >= 0);

  add2->mantissa = add2->mantissa >> getMSB;
  add2->mantissa = ((add2->mantissa & (~(0x1 << 23))) | 0x0);
  add2->exponent = add2->exponent + getMSB;
  return *add2;
} // isNegAndSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const

const MyFloat MyFloat::justSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const
{
  unsigned int Exp1 = add1->exponent;
  unsigned int Mant1;
  add1->mantissa = add1->mantissa << diffExp;
  add1->exponent = Exp1 - diffExp;
  Exp1 = add1->exponent;

  if (timeToCarry(add1->mantissa,add2->mantissa) == 1)
  {
    add1->mantissa = add1->mantissa >> 1;
    add2->mantissa = add2->mantissa >> 1;
    Exp1++;
  } // if flag is true.

  add1->mantissa = add1->mantissa + add2->mantissa;
  int i = 31;
  Mant1 = add1->mantissa;
  do
  {
    if (!(Mant1 & (0x1 << i)))
      i--;
    else
    {
      getMSB = i - 23;
      i--;
      break;
    }

  } while(i >= 0);

  Exp1 = add1->exponent;
  add1->mantissa = add1->mantissa >> getMSB;
  Mant1 = add1->mantissa;
  add1->mantissa = (Mant1 &(~(0x1 << 23))) | 0x0;
  add1->exponent = Exp1 + getMSB;
  return *add1;
} //justSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const

const MyFloat MyFloat::notNegAndSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const
{
  unsigned int Mant2;
  add2->mantissa = add2->mantissa << diffExp;
  add2->exponent = add2->exponent - diffExp;

  if (add2->mantissa > add1->mantissa)
    add2->mantissa = add2->mantissa - add1->mantissa;

  if (add1->mantissa > add2->mantissa)
    add2->mantissa = add1->mantissa - add2->mantissa;

  int i = 31;
  Mant2 = add2->mantissa;
  do
  {
    if (!(Mant2 & (0x1 << i)))
      i--;
    else
    {
      getMSB = i - 23;
      i--;
      break;
    }

  } while(i >= 0);

  add2->mantissa = add2->mantissa >> getMSB;
  Mant2 = add2->mantissa;
  add2->mantissa = (~(((0x1 << 23) | ~Mant2))) | 0x0;
  add2->sign = greater;
  add2->exponent = add2->exponent + getMSB;
  return *add2;
} // notNegAndSameSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const

const MyFloat MyFloat::notNegAndDiffSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const
{
  unsigned int Mant1;
  add1->mantissa = add1->mantissa << diffExp;
  add1->exponent -= diffExp;

  if (add2->mantissa > add1->mantissa)
    add1->mantissa = add2->mantissa - add1->mantissa;

  if (add1->mantissa > add2->mantissa)
    add1->mantissa = add1->mantissa - add2->mantissa;

  int i = 31;
  Mant1 = add1->mantissa;
  do
  {
    if (!(Mant1 & (0x1 << i)))
      i--;
    else
    {
      getMSB = i - 23;
      i--;
      break;
    }

  } while(i >= 0);

  add1->mantissa = add1->mantissa >> getMSB;
  Mant1 = add1->mantissa;
  add1->mantissa = (Mant1 & (~(0x1 << 23))) | 0x0;
  add1->sign = greater;
  add1->exponent = add1->exponent + getMSB;
  return *add1;
} // notNegAndDiffSign(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const


const MyFloat MyFloat::notNegAndSameSignCarry(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const
{
  add2->mantissa = add2->mantissa >> diffExp;
  add2->exponent += diffExp;
  unsigned int num1 =  add1->mantissa, num2 = add2->mantissa;

  if (timeToCarry(num1, num2) == 1) // If flag is true
  {
    add1->mantissa = (add1->mantissa) >> 1;
    add2->mantissa = add2->mantissa >> 1;
    add2->exponent++;
  } // if flag is true

  add2->mantissa = add1->mantissa + add2->mantissa;
  unsigned int Mant = add2->mantissa;
  int i = 31;
  do
  {
    if (!(Mant & (0x1 << i)))
      i--;

    else
    {
      getMSB = i -23;
      i--;
      if (getMSB < 0)
        getMSB = -(getMSB);
      break;
    }

  } while (i >= 0);

  add2->mantissa = (add2->mantissa >> getMSB);
  Mant = add2->mantissa;
  add2->exponent = add2->exponent + getMSB;
  add2->mantissa = ((Mant & (~(0x1 << 23))) | 0x0);
  return *add2;
} // notNegAndSameSignCarry(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const


const MyFloat MyFloat::isNegAndSameSignCarry(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const
{
  unsigned int num1, num2, Mant1;
  add1->mantissa = add1->mantissa >> diffExp;
  add1->exponent += diffExp;
  num1 = add1->mantissa;
  num2 = add2->mantissa;
  if (timeToCarry(num1, num2) == 1)
  {
    add1->mantissa = add1->mantissa >> 0x1;
    add2->mantissa = add2->mantissa >> 0x1;
    add1->exponent++;
  } // if flag is true.

  add1->mantissa = add1->mantissa + add2->mantissa;
  int i = 31;
  do
  {
    if (!(add1->mantissa & (0x1 << i)))
      i--;

    else
    {
      getMSB = i - 23;
      i--;
      if (getMSB < 0)
        getMSB = -(getMSB);
      break;
    }

  } while(i >= 0);

  add1->mantissa = add1->mantissa >> getMSB;
  add1->exponent = add1->exponent + getMSB;
  Mant1 = add1->mantissa;
  add1->mantissa = ((Mant1 & (~(0x1 << 23))) | 0x0);
  return *add1;
} // isNegAndSameSignCarry(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp) const


const MyFloat MyFloat::notNegAndDiffSignBorrow(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const
{
  unsigned int Mant = add1->mantissa;
  add2->mantissa = add2->mantissa >> diffExp;
  add2->exponent = add2->exponent + diffExp;

  if (add2->mantissa > add1->mantissa)
  {
    if (timeToBorrow(Mant, diffExp) == 1)
      add2->mantissa--;

    add2->mantissa = add2->mantissa - add1->mantissa;
  }

  if(add1->mantissa > add2->mantissa)
  {
    if (timeToBorrow(Mant, diffExp) == 1)
      add1->mantissa--;

    add2->mantissa = add1->mantissa - add2->mantissa;
  }

  int i = 31;
  do
  {
    if (!(add2->mantissa & (0x1 << i)))
      i--;

    else
    {
      getMSB = i - 23;
      i--;
      break;
    }
  } while(i >= 0);

  add2->mantissa = add2->mantissa >> getMSB;
  Mant = add1->mantissa;
  add2->mantissa = (Mant & (~(0x1 << 23))) | 0x0;
  add2->sign = greater;
  add2->exponent = add2->exponent + getMSB;
  return *add2;
} // notNegAndDiffSignBorrow(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const


const MyFloat MyFloat::isNegAndDiffSignBorrow(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const
{
  unsigned int Mant = add1->mantissa;
  add1->mantissa = add1->mantissa >> diffExp;
  add1->exponent += diffExp;
  if (add2->mantissa > add1->mantissa)
  {
    if (timeToBorrow(Mant, diffExp) == 1)
      add2->mantissa--;

    add1->mantissa = add2->mantissa - add1->mantissa;
  }

  else
  {
    if (timeToBorrow(Mant, diffExp) == 1)
      add1->mantissa--;

    add1->mantissa = add1->mantissa - add2->mantissa;
  }

  int i = 31;
  do
  {
    if (!(add1->mantissa & (0x1 << i)))
      i--;

    else
    {
      getMSB = i - 23;
      i--;
      break;
    }
  } while(i >= 0); // Do - while

  add1->mantissa = add1->mantissa >> getMSB;
  Mant = add1->mantissa;
  add1->mantissa = (Mant & (~(0x1 << 23))) | 0x0;
  add1->sign = greater;
  add1->exponent = add2->exponent + getMSB;
  return *add1;
} // isNegAndDiffSignBorrow(MyFloat *add1, MyFloat *add2, int getMSB, const int diffExp, const int greater) const
