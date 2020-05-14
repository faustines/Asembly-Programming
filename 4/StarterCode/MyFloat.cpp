#include "MyFloat.h"

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
	sign = rhs.sign;
	exponent = rhs.exponent;
	mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
	//this function is complete. No need to modify it.
	strm << f.packFloat();
	return strm;
}

MyFloat MyFloat::operator+(const MyFloat& rhs) const{

    MyFloat u = MyFloat(*this);
    MyFloat v = MyFloat(rhs);

    //STEP 1: Adding the hidden bit to manitssa if exponent is non-zero
    //(Checking for 0's)

    if(u.exponent != 0){
        u.mantissa += 0x800000;
    }
	if(v.exponent != 0){
	    v.mantissa += 0x800000;
	}

	u.mantissa <<= 1;
	u.exponent--;
	v.mantissa <<= 1;
	v.exponent--;

	//STEP 2: Aligning the Mantissas

	while(u.exponent != v.exponent){ //while not aligned
	    if(u.exponent > v.exponent){
	        v.exponent++; //increase v's exponent to match
	        v.mantissa >>= 1; //divide v's mantissa by 2 as a result
	    }
	    else{ //v.exponent > u.exponent
	        u.exponent++; //increase u's exponent to match
	        u.mantissa >>= 1; //divide u's mantissa by 2 as a result
	    }
	}

	//STEP 3: Add or Subtract the Mantissas
	if(u.sign == v.sign){ //both have 0 as sign bit or 1
	    u.sign = u.sign; //u's sign doesn't change
	    u.mantissa += v.mantissa; //simple addition of mantissas
	}
	else{ //sign bits don't match
	    if(u.mantissa > v.mantissa){ //u is bigget than v
	        u.sign = u.sign; //u's sign doesn't change
	        u.mantissa =  u.mantissa - v.mantissa;
	    }
	    else{ //v.mantissa > u.mantissa (v is bigger than u)
	        u.sign = v.sign; //u's sign is whatever v's sign is
	        u.mantissa = v.mantissa - u.mantissa;
	    }
	}

	//STEP 4: Normalize the Result
	if(u.mantissa == 0){ //SPECIAL CASE WHEN 0!
	    u.exponent = 0;
	    u.sign = 0;
	    return u;
	}
    //Truncate to be 23 most significant bits
    while(u.mantissa >= 0x1000000){
        u.exponent++; //increase exponent
        u.mantissa >>= 1; //divide mantissa by 2 as a result
    }
    //Number of bits in mantissa is too little
    while(u.mantissa < 0x800000){
        u.mantissa <<= 1; //multiply mantissa by 2
        u.exponent--; //decrease exponent as a result
    }
    u.mantissa -= 0x800000; //remove the 1. to extract the mantissa
	return u;
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{

    /*Subtraction is equivalent to doing negative addition,
    so we can rely on the operator+ function and just flip the
    sign for the rhs argument and call upon the operator+ function.
    */
    MyFloat float_sub(rhs);
    if(float_sub.sign == 1){
        float_sub.sign = 0;
    }
    else{ //float_sub.sign == 0
        float_sub.sign = 1;
    }
	return operator+(float_sub); //calls operator+ to do "negation addition"
}

bool MyFloat::operator==(const float rhs) const{

    MyFloat float_cpy = MyFloat(rhs);
    if(this->sign == float_cpy.sign &&
    this->exponent == float_cpy.exponent &&
    this->mantissa == float_cpy.mantissa){
        return true;
    }
	return false;
}


void MyFloat::unpackFloat(float f) {
	//extracts the fields of f into sign, exponent, and mantissa

	__asm__(
    	//Sign: f >> 31
    	"shr $31, %%eax;"
    	//Exponent: (f << 1) >> 24
    	"shl $1, %%ebx;"
    	"shr $24, %%ebx;"
    	//Mantissa: (f << 9) >> 9
    	"shl $9, %%ecx;"
    	"shr $9, %%ecx":
    	"=a" (sign), "=b" (exponent), "=c" (mantissa):
    	"a" (f), "b" (f), "c" (f):
    	"cc"
	);
}//unpackFloat

float MyFloat::packFloat() const{
  //returns the floating point number represented by this
  float f = 0;

  __asm__(
      //Mantissa = Bits 22 - 0
      "shl $31, %[signBit];"                //Sign = Bit 31
      "shl $23, %[exponentBit];"            //Exponent = Bits 30 - 23
      "addl %[signBit], %[total];"          //float += sign
      "addl %[exponentBit], %[total];"      //float += exponent
      "addl %[mantissaBit], %[total]":      //float += mantissa
      [total] "+r" (f):
      [signBit] "r" (sign),
      [exponentBit] "r" (exponent),
      [mantissaBit] "r" (mantissa):
      "cc"
    );
  return f;
}//packFloat
