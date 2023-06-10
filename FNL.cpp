/*
Fast Natural Log - https://github.com/StevenChristy/FastNaturalLog

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

class Bool
{
	bool scalar;
public:
	inline Bool(bool b)
		: scalar(b)
	{
	}

	inline operator bool()
	{
		return scalar;
	}
};

class Int32
{
	int32_t scalar;
public:
	inline Int32(int32_t i)
		: scalar(i)
	{
	}

	inline operator int32_t()
	{
		return scalar;
	}

	inline Bool operator==(const int32_t &i)
	{
		return scalar == i;
	}

	inline float ConvertToFloat()
	{
		return static_cast<float>(scalar);	
	}
	inline float CastToFloat()
	{
		return *((float*)&scalar); 
	}
};

class Int64
{
	int64_t scalar;
public:
	inline Int64(int64_t i)
		: scalar(i)
	{
	}

	inline operator int64_t()
	{
		return scalar;
	}

	inline Bool operator==(const int64_t &i)
	{
		return scalar == i;
	}

	inline double ConvertToFloat()
	{
		return static_cast<double>(scalar);	
	}
	inline double CastToFloat()
	{
		return *((double*)&scalar); 
	}
};


class FP32
{
	float scalar;
public:
	using IntType = Int32;
	
	constexpr static int32_t One = 0x3F800000;
	constexpr static int32_t MantissaMask = 0x007FFFFF;
	constexpr static int32_t ExponentMask = 0x7F800000;
	constexpr static int32_t AbsValueMask = 0x7FFFFFFF;
	constexpr static int32_t SignBit = ~0x7FFFFFFF;
	constexpr static int32_t MantissaBits = 23;
	constexpr static int32_t BaseExponent = 127;
	constexpr static int32_t NegativeNAN = ~0x003FFFFF;
	constexpr static int32_t NegativeINF = ~0x007FFFFF;
	
	inline FP32(float f)
		: scalar(f)
	{
		
	}

	explicit inline FP32(double d)
		: scalar(static_cast<float>(d))
	{
		
	}

	inline FP32 &operator *=(const FP32 &val)
	{
		scalar *= val.scalar;
		return *this;
	}

	inline FP32 &operator +=(const FP32 &val)
	{
		scalar += val.scalar;
		return *this;
	}
	
	
	inline operator float()
	{
		return scalar;
	}

	inline int32_t CastToInt()
	{
		return *((int32_t*)&scalar); 
	}
};

class FP64
{
	double scalar;
public:
	using IntType = Int64;
	
	constexpr static int64_t One = 0x3FF0000000000000LL;
	constexpr static int64_t MantissaMask = 0x000FFFFFFFFFFFFFLL;
	constexpr static int64_t ExponentMask = 0x7FF0000000000000LL;
	constexpr static int64_t AbsValueMask = 0x7FFFFFFFFFFFFFFFLL;
	constexpr static int64_t SignBit = ~0x7FFFFFFFFFFFFFFFLL;
	constexpr static int64_t MantissaBits = 52;
	constexpr static int64_t BaseExponent = 1023;
	constexpr static int64_t NegativeNAN = ~0x0007FFFFFFFFFFFFLL;
	constexpr static int64_t NegativeINF = ~0x000FFFFFFFFFFFFFLL;
	
	inline FP64(double d)
		: scalar(d)
	{
		
	}

	inline FP64 &operator *=(const FP64 &val)
	{
		scalar *= val.scalar;
		return *this;
	}

	inline FP64 &operator +=(const FP64 &val)
	{
		scalar += val.scalar;
		return *this;
	}
	
	
	inline operator double()
	{
		return scalar;
	}

	inline int64_t CastToInt()
	{
		return *((int64_t*)&scalar); 
	}
};

FP32 NaturalLog(FP32 x)
{
	using Float = FP32; // This function could be used for FP64 but is not as precise as the STL version of the double precision std::log
	using Int = typename Float::IntType;

	Bool Negative = Int(x.CastToInt() & Int(Float::SignBit)) == Int(Float::SignBit);
	if (Negative)
		return Int(Float::NegativeNAN).CastToFloat();
	Bool Exceptional = (x.CastToInt() & Int(Float::ExponentMask)) == Int(Float::ExponentMask);
	if (Exceptional)
		return x;
	Bool Zero = Int(x.CastToInt() & Int(Float::AbsValueMask)) == Int(0);
	if (Zero)
		return Int(Float::NegativeINF).CastToFloat();
	
	Float nx = Int((x.CastToInt() & Int(Float::MantissaMask)) | Int(Float::One)).CastToFloat(); 
	Float z = (nx - Float(1.0)) / (nx + Float(1.0));
	Float z_sq = z * z;
	Float Result = z;
	z *= z_sq;
	Result += z * Float(1.0 / 3.0);
	z *= z_sq;
	Result += z * Float(1.0 / 5.0);
	z *= z_sq;
	Result += z * Float(1.0 / 7.0);
	z *= z_sq;
	Result += z * Float(1.0 / 9.0);

	// Optional
	z *= z_sq;
	Result += z * Float(1.0 / 11.0);
	//
	
	Result += Result;
	Result += Int(((x.CastToInt() & Int(Float::ExponentMask)) >> Float::MantissaBits) - Int(Float::BaseExponent)).ConvertToFloat() * Float(0.69314718056);
	
	return Result;
}

FP64 NaturalLog(FP64 x)
{
	using Float = FP64;
	using Int = typename Float::IntType;

	Bool Negative = Int(x.CastToInt() & Int(Float::SignBit)) == Int(Float::SignBit);
	if (Negative)
		return Int(Float::NegativeNAN).CastToFloat();
	Bool Exceptional = (x.CastToInt() & Int(Float::ExponentMask)) == Int(Float::ExponentMask);
	if (Exceptional)
		return x;
	Bool Zero = Int(x.CastToInt() & Int(Float::AbsValueMask)) == Int(0);
	if (Zero)
		return Int(Float::NegativeINF).CastToFloat();
	
	Float nx = Int((x.CastToInt() & Int(Float::MantissaMask)) | Int(Float::One)).CastToFloat(); 
	Float z = (nx - Float(1.0)) / (nx + Float(1.0));
	Float z_sq = z * z;
	Float Result = z;
	z *= z_sq;
	Result += z * Float(1.0 / 3.0);
	z *= z_sq;
	Result += z * Float(1.0 / 5.0);
	z *= z_sq;
	Result += z * Float(1.0 / 7.0);
	z *= z_sq;
	Result += z * Float(1.0 / 9.0);
	z *= z_sq;
	Result += z * Float(1.0 / 11.0);
	
	// Optional
	z *= z_sq;
	Result += z * Float(1.0 / 13.0);
	z *= z_sq;
	Result += z * Float(1.0 / 15.0);
	z *= z_sq;
	Result += z * Float(1.0 / 17.0);
	z *= z_sq;
	Result += z * Float(1.0 / 19.0);
	//
	
	Result += Result;
	Result += Int(((x.CastToInt() & Int(Float::ExponentMask)) >> Float::MantissaBits) - Int(Float::BaseExponent)).ConvertToFloat() * Float(0.69314718056);
	
	return Result;
}


int main() 
{
	double tests[] = {std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN(), -std::numeric_limits<double>::quiet_NaN(),
    		-2.0, -1.5, -1., -0.5, 0.0, 0.0000001, 0.01, 0.05, 0.1, 0.2, 0.5, 0.6, 0.85, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2., 2.1, 2.2, 100.0, 1000.0, 10000.0};

	std::cout << std::setw(10) << " " << std::setw(11) << std::right << "FP32" << std::setw(11) << "" << std::setw(18) << std::right << "FP64" << std::endl; 
	std::cout << std::setw(10) << std::right << "LN" << std::setw(11) << std::right << "FNL" << std::setw(11) << std::right << "STL" << std::setw(18) << std::right << "FNL" << std::setw(18) << std::right << "STL" << std::endl; 
	
	int numTests = sizeof(tests) / sizeof(double);
	for (int i = 0; i < numTests; i++) 
	{
        	float fr = NaturalLog(FP32(tests[i]));
		double dr = NaturalLog(FP64(tests[i]));
		std::cout << std::setprecision(6) << std::setw(10) << tests[i] << std::setw(11) << fr << std::setw(11) << std::log(static_cast<float>(tests[i])) << std::setprecision(12) << std::setw(18) << dr << std::setw(18) <<  std::log(tests[i]) << std::endl; 
	}
	return 0;
}
