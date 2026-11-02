#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <string>
#include <vector>
#include <cstdint>

// Simple arbitrary-precision unsigned integer used by the PopCap
// registration and signature validation code.  Numbers are stored as
// 32-bit limbs, least significant limb first.
class BigInt
{
public:
	std::vector<uint32_t>	mLimbs;

public:
	BigInt();
	BigInt(uint32_t theValue);
	explicit BigInt(const std::string& theHexString);

	bool					IsZero() const;
	int						GetBitLength() const;
	void					Trim();

	std::string				ToString() const;

	bool operator==(const BigInt& theOther) const;
	bool operator!=(const BigInt& theOther) const { return !(*this == theOther); }
	bool operator<(const BigInt& theOther) const;

	BigInt operator+(const BigInt& theOther) const;
	BigInt operator-(const BigInt& theOther) const;
	BigInt operator*(const BigInt& theOther) const;
	BigInt& operator+=(const BigInt& theOther);
	BigInt& operator*=(const BigInt& theOther);

	BigInt					DivMod(const BigInt& theDivisor, BigInt* theRemainder) const;
	BigInt					Mod(const BigInt& theDivisor) const;

	BigInt					ModPow(const BigInt& theExponent, const BigInt& theModulus) const;

	BigInt					ShiftLeft(int theBits) const;
	BigInt					ShiftRight(int theBits) const;
};

BigInt HashData(const unsigned char* theData, int theLength, int theNumBits);
BigInt HashString(const std::string& theString, int theNumBits);
BigInt KeyToInt(const std::string& theKey);

#endif // __BIGINT_H__
