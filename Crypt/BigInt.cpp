#include "BigInt.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>

namespace
{

struct MD5Context
{
	uint32_t	mState[4];
	uint32_t	mCount[2];
	unsigned char	mBuffer[64];
};

static uint32_t RotateLeft(uint32_t x, int c)
{
	return (x << c) | (x >> (32 - c));
}

static void MD5Transform(uint32_t state[4], const unsigned char block[64])
{
	static const uint32_t K[64] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};
	static const int S[64] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

	uint32_t M[16];
	for (int i = 0; i < 16; i++)
		M[i] = (uint32_t) block[i * 4] |
		       ((uint32_t) block[i * 4 + 1] << 8) |
		       ((uint32_t) block[i * 4 + 2] << 16) |
		       ((uint32_t) block[i * 4 + 3] << 24);

	uint32_t A = state[0];
	uint32_t B = state[1];
	uint32_t C = state[2];
	uint32_t D = state[3];

	for (int i = 0; i < 64; i++)
	{
		uint32_t F;
		int g;
		if (i < 16)
		{
			F = (B & C) | (~B & D);
			g = i;
		}
		else if (i < 32)
		{
			F = (D & B) | (~D & C);
			g = (5 * i + 1) & 15;
		}
		else if (i < 48)
		{
			F = B ^ C ^ D;
			g = (3 * i + 5) & 15;
		}
		else
		{
			F = C ^ (B | ~D);
			g = (7 * i) & 15;
		}

		uint32_t tmp = D;
		D = C;
		C = B;
		B = B + RotateLeft(A + F + K[i] + M[g], S[i]);
		A = tmp;
	}

	state[0] += A;
	state[1] += B;
	state[2] += C;
	state[3] += D;
}

static void MD5Init(MD5Context* ctx)
{
	ctx->mCount[0] = ctx->mCount[1] = 0;
	ctx->mState[0] = 0x67452301;
	ctx->mState[1] = 0xefcdab89;
	ctx->mState[2] = 0x98badcfe;
	ctx->mState[3] = 0x10325476;
}

static void MD5Update(MD5Context* ctx, const unsigned char* input, size_t length)
{
	size_t index = (ctx->mCount[0] >> 3) & 63;
	ctx->mCount[0] += (uint32_t) (length << 3);
	if (ctx->mCount[0] < (uint32_t) (length << 3))
		ctx->mCount[1]++;
	ctx->mCount[1] += (uint32_t) (length >> 29);

	size_t partLen = 64 - index;
	size_t i = 0;
	if (length >= partLen)
	{
		memcpy(&ctx->mBuffer[index], input, partLen);
		MD5Transform(ctx->mState, ctx->mBuffer);
		for (i = partLen; i + 63 < length; i += 64)
			MD5Transform(ctx->mState, &input[i]);
		index = 0;
	}

	memcpy(&ctx->mBuffer[index], &input[i], length - i);
}

static void MD5Final(unsigned char digest[16], MD5Context* ctx)
{
	static const unsigned char PADDING[64] = { 0x80 };
	unsigned char bits[8];

	for (int i = 0; i < 4; i++)
	{
		bits[i] = (unsigned char) (ctx->mCount[0] >> (8 * i));
		bits[i + 4] = (unsigned char) (ctx->mCount[1] >> (8 * i));
	}

	size_t index = (ctx->mCount[0] >> 3) & 63;
	size_t padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update(ctx, PADDING, padLen);
	MD5Update(ctx, bits, 8);

	for (int i = 0; i < 4; i++)
	{
		digest[i * 4] = (unsigned char) (ctx->mState[i]);
		digest[i * 4 + 1] = (unsigned char) (ctx->mState[i] >> 8);
		digest[i * 4 + 2] = (unsigned char) (ctx->mState[i] >> 16);
		digest[i * 4 + 3] = (unsigned char) (ctx->mState[i] >> 24);
	}
}

} // anonymous namespace

BigInt::BigInt()
{
}

BigInt::BigInt(uint32_t theValue)
{
	if (theValue != 0)
		mLimbs.push_back(theValue);
}

BigInt::BigInt(const std::string& theHexString)
{
	size_t aPos = 0;
	if (theHexString.size() >= 2 && theHexString[0] == '0' &&
		(theHexString[1] == 'x' || theHexString[1] == 'X'))
	{
		aPos = 2;
	}

	for (size_t i = aPos; i < theHexString.size(); i++)
	{
		char c = theHexString[i];
		uint32_t aDigit;
		if (c >= '0' && c <= '9')
			aDigit = c - '0';
		else if (c >= 'a' && c <= 'f')
			aDigit = c - 'a' + 10;
		else if (c >= 'A' && c <= 'F')
			aDigit = c - 'A' + 10;
		else
			continue;

		*this = ShiftLeft(4) + BigInt(aDigit);
	}
}

bool BigInt::IsZero() const
{
	return mLimbs.empty();
}

void BigInt::Trim()
{
	while (!mLimbs.empty() && mLimbs.back() == 0)
		mLimbs.pop_back();
}

int BigInt::GetBitLength() const
{
	if (mLimbs.empty())
		return 0;

	uint32_t aTop = mLimbs.back();
	int aBits = (int) (mLimbs.size() - 1) * 32;
	while (aTop != 0)
	{
		aBits++;
		aTop >>= 1;
	}
	return aBits;
}

std::string BigInt::ToString() const
{
	if (mLimbs.empty())
		return "0";

	std::string aResult;
	for (int i = (int) mLimbs.size() - 1; i >= 0; i--)
	{
		char aBuf[16];
		if (i == (int) mLimbs.size() - 1)
			sprintf(aBuf, "%X", mLimbs[i]);
		else
			sprintf(aBuf, "%08X", mLimbs[i]);
		aResult += aBuf;
	}
	return aResult;
}

bool BigInt::operator==(const BigInt& theOther) const
{
	if (mLimbs.size() != theOther.mLimbs.size())
		return false;
	for (size_t i = 0; i < mLimbs.size(); i++)
		if (mLimbs[i] != theOther.mLimbs[i])
			return false;
	return true;
}

bool BigInt::operator<(const BigInt& theOther) const
{
	if (mLimbs.size() != theOther.mLimbs.size())
		return mLimbs.size() < theOther.mLimbs.size();
	for (int i = (int) mLimbs.size() - 1; i >= 0; i--)
	{
		if (mLimbs[i] != theOther.mLimbs[i])
			return mLimbs[i] < theOther.mLimbs[i];
	}
	return false;
}

BigInt BigInt::operator+(const BigInt& theOther) const
{
	BigInt aResult;
	size_t aSize = std::max(mLimbs.size(), theOther.mLimbs.size());
	aResult.mLimbs.resize(aSize, 0);
	uint64_t aCarry = 0;
	for (size_t i = 0; i < aSize; i++)
	{
		uint64_t aSum = aCarry;
		if (i < mLimbs.size())
			aSum += mLimbs[i];
		if (i < theOther.mLimbs.size())
			aSum += theOther.mLimbs[i];
		aResult.mLimbs[i] = (uint32_t) aSum;
		aCarry = aSum >> 32;
	}
	if (aCarry != 0)
		aResult.mLimbs.push_back((uint32_t) aCarry);
	return aResult;
}

BigInt BigInt::operator-(const BigInt& theOther) const
{
	BigInt aResult;
	aResult.mLimbs.resize(mLimbs.size(), 0);
	int64_t aBorrow = 0;
	for (size_t i = 0; i < mLimbs.size(); i++)
	{
		int64_t aDiff = (int64_t) mLimbs[i] - aBorrow;
		if (i < theOther.mLimbs.size())
			aDiff -= theOther.mLimbs[i];
		if (aDiff < 0)
		{
			aDiff += ((int64_t) 1 << 32);
			aBorrow = 1;
		}
		else
			aBorrow = 0;
		aResult.mLimbs[i] = (uint32_t) aDiff;
	}
	aResult.Trim();
	return aResult;
}

BigInt BigInt::operator*(const BigInt& theOther) const
{
	BigInt aResult;
	if (mLimbs.empty() || theOther.mLimbs.empty())
		return aResult;

	aResult.mLimbs.resize(mLimbs.size() + theOther.mLimbs.size(), 0);
	for (size_t i = 0; i < mLimbs.size(); i++)
	{
		uint64_t aCarry = 0;
		for (size_t j = 0; j < theOther.mLimbs.size(); j++)
		{
			uint64_t aProduct = (uint64_t) mLimbs[i] * theOther.mLimbs[j] +
				aResult.mLimbs[i + j] + aCarry;
			aResult.mLimbs[i + j] = (uint32_t) aProduct;
			aCarry = aProduct >> 32;
		}
		aResult.mLimbs[i + theOther.mLimbs.size()] = (uint32_t) aCarry;
	}
	aResult.Trim();
	return aResult;
}

BigInt& BigInt::operator+=(const BigInt& theOther)
{
	*this = *this + theOther;
	return *this;
}

BigInt& BigInt::operator*=(const BigInt& theOther)
{
	*this = *this * theOther;
	return *this;
}

BigInt BigInt::ShiftLeft(int theBits) const
{
	if (theBits <= 0 || mLimbs.empty())
		return *this;

	int aLimbShift = theBits / 32;
	int aBitShift = theBits % 32;
	BigInt aResult;
	aResult.mLimbs.resize(mLimbs.size() + aLimbShift + 1, 0);

	for (size_t i = 0; i < mLimbs.size(); i++)
	{
		uint64_t aValue = (uint64_t) mLimbs[i] << aBitShift;
		aResult.mLimbs[i + aLimbShift] |= (uint32_t) aValue;
		aResult.mLimbs[i + aLimbShift + 1] |= (uint32_t) (aValue >> 32);
	}
	aResult.Trim();
	return aResult;
}

BigInt BigInt::ShiftRight(int theBits) const
{
	if (theBits <= 0 || mLimbs.empty())
		return *this;

	int aLimbShift = theBits / 32;
	int aBitShift = theBits % 32;
	if (aLimbShift >= (int) mLimbs.size())
		return BigInt();

	BigInt aResult;
	aResult.mLimbs.resize(mLimbs.size() - aLimbShift, 0);
	for (size_t i = 0; i < aResult.mLimbs.size(); i++)
	{
		uint64_t aValue = mLimbs[i + aLimbShift];
		if (aBitShift != 0 && i + aLimbShift + 1 < mLimbs.size())
			aValue |= (uint64_t) mLimbs[i + aLimbShift + 1] << 32;
		aResult.mLimbs[i] = (uint32_t) (aValue >> aBitShift);
	}
	aResult.Trim();
	return aResult;
}

BigInt BigInt::DivMod(const BigInt& theDivisor, BigInt* theRemainder) const
{
	if (theDivisor.IsZero())
	{
		if (theRemainder)
			*theRemainder = BigInt();
		return BigInt();
	}

	BigInt aQuotient;
	BigInt aRemainder;
	int aBits = GetBitLength();
	aQuotient.mLimbs.resize(mLimbs.size(), 0);

	for (int i = aBits - 1; i >= 0; i--)
	{
		aRemainder = aRemainder.ShiftLeft(1);
		if ((mLimbs[i / 32] >> (i % 32)) & 1)
			aRemainder = aRemainder + BigInt(1);
		aRemainder.Trim();

		if (!(aRemainder < theDivisor))
		{
			aRemainder = aRemainder - theDivisor;
			aQuotient.mLimbs[i / 32] |= (uint32_t) 1 << (i % 32);
		}
	}

	aQuotient.Trim();
	if (theRemainder)
		*theRemainder = aRemainder;
	return aQuotient;
}

BigInt BigInt::Mod(const BigInt& theDivisor) const
{
	BigInt aRemainder;
	DivMod(theDivisor, &aRemainder);
	return aRemainder;
}

BigInt BigInt::ModPow(const BigInt& theExponent, const BigInt& theModulus) const
{
	if (theModulus.IsZero())
		return BigInt();

	BigInt aResult(1);
	BigInt aBase = Mod(theModulus);
	BigInt anExponent = theExponent;

	while (!anExponent.IsZero())
	{
		if (!anExponent.mLimbs.empty() && (anExponent.mLimbs[0] & 1))
			aResult = (aResult * aBase).Mod(theModulus);

		anExponent = anExponent.ShiftRight(1);
		if (!anExponent.IsZero())
			aBase = (aBase * aBase).Mod(theModulus);
	}

	return aResult.Mod(theModulus);
}

BigInt HashData(const unsigned char* theData, int theLength, int theNumBits)
{
	MD5Context aContext;
	MD5Init(&aContext);
	MD5Update(&aContext, theData, theLength);

	unsigned char aDigest[16];
	MD5Final(aDigest, &aContext);

	BigInt aResult;
	for (int i = 0; i < 16; i++)
	{
		aResult = aResult.ShiftLeft(8) + BigInt(aDigest[i]);
	}

	// Pad with nibbles until the value is at least the requested size,
	// then discard the low bits so that exactly theNumBits remain.
	int aNumNibbles = (theNumBits + 3) / 4 - 32;
	for (int i = 0; i < aNumNibbles; i++)
		aResult = aResult.ShiftLeft(4) + BigInt((uint32_t) (i % 16));

	int aExcessBits = aResult.GetBitLength() - theNumBits;
	if (aExcessBits > 0)
		aResult = aResult.ShiftRight(aExcessBits);

	return aResult;
}

BigInt HashString(const std::string& theString, int theNumBits)
{
	return HashData((const unsigned char*) theString.data(),
		(int) theString.size(), theNumBits);
}

BigInt KeyToInt(const std::string& theKey)
{
	static const char* aKeyChars = "234679ACDEFGHJKLMNPQRTUVWXYZ";

	std::string aCleanKey;
	for (size_t i = 0; i < theKey.size(); i++)
	{
		char c = theKey[i];
		if (c == ' ')
			continue;
		if (c == '1')
			aCleanKey += 'L';
		else if (c == '0' || c == 'o' || c == 'O')
			aCleanKey += 'Q';
		else
			aCleanKey += (char) toupper((unsigned char) c);
	}

	BigInt aResult;
	int aLength = (int) aCleanKey.size();
	for (int i = 0; i < aLength; i++)
	{
		if ((aLength - i) % 6 == 0)
		{
			if (aCleanKey[i] != '-')
				return BigInt();
		}
		else
		{
			const char* aPos = strchr(aKeyChars, toupper((unsigned char) aCleanKey[i]));
			if (aPos == NULL)
				return BigInt();

			int aValue = (int) (aPos - aKeyChars);
			aResult = aResult * BigInt(28) + BigInt(aValue);
		}
	}

	return aResult;
}
