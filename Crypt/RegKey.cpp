#include "RegKey.h"

// Extracted from the original executable.  Each pair maps a source
// character (usually an accented Latin letter) to its ASCII equivalent.
const char gRegKeyTranslationTable[][2] =
{
	{ (char) 0x80, 'C' }, { (char) 0x81, 'u' }, { (char) 0x82, 'e' },
	{ (char) 0x83, 'a' }, { (char) 0x84, 'a' }, { (char) 0x85, 'a' },
	{ (char) 0x86, 'a' }, { (char) 0x87, 'c' }, { (char) 0x88, 'e' },
	{ (char) 0x89, 'e' }, { (char) 0x8A, 'e' }, { (char) 0x8B, 'i' },
	{ (char) 0x8C, 'i' }, { (char) 0x8D, 'i' }, { (char) 0x8E, 'A' },
	{ (char) 0x8F, 'A' }, { (char) 0x90, 'E' }, { (char) 0x91, 'a' },
	{ (char) 0x92, 'A' }, { (char) 0x93, 'o' }, { (char) 0x94, 'o' },
	{ (char) 0x95, 'o' }, { (char) 0x96, 'u' }, { (char) 0x97, 'u' },
	{ (char) 0x98, 'y' }, { (char) 0x99, 'O' }, { (char) 0x9A, 'U' },
	{ (char) 0xA0, 'a' }, { (char) 0xA1, 'i' }, { (char) 0xA2, 'o' },
	{ (char) 0xA3, 'u' }, { (char) 0xA4, 'n' }, { (char) 0xA5, 'N' },
	{ (char) 0xC0, 'A' }, { (char) 0xC1, 'A' }, { (char) 0xC2, 'A' },
	{ (char) 0xC3, 'A' }, { (char) 0xC4, 'A' }, { (char) 0xC5, 'A' },
	{ (char) 0xC6, 'A' }, { (char) 0xC7, 'C' }, { (char) 0xC8, 'E' },
	{ (char) 0xC9, 'E' }, { (char) 0xCA, 'E' }, { (char) 0xCB, 'E' },
	{ (char) 0xCC, 'I' }, { (char) 0xCD, 'I' }, { (char) 0xCE, 'I' },
	{ (char) 0xCF, 'I' }, { (char) 0xD0, 'D' }, { (char) 0xD1, 'N' },
	{ (char) 0xD2, 'O' }, { (char) 0xD3, 'O' }, { (char) 0xD4, 'O' },
	{ (char) 0xD5, 'O' }, { (char) 0xD6, 'O' }, { (char) 0xD8, 'O' },
	{ (char) 0xD9, 'U' }, { (char) 0xDA, 'U' }, { (char) 0xDB, 'U' },
	{ (char) 0xDC, 'U' }, { (char) 0xDD, 'Y' }, { (char) 0xDE, 'b' },
	{ (char) 0xDF, 'B' }, { (char) 0xE0, 'a' }, { (char) 0xE1, 'a' },
	{ (char) 0xE2, 'a' }, { (char) 0xE3, 'a' }, { (char) 0xE4, 'a' },
	{ (char) 0xE5, 'a' }, { (char) 0xE6, 'c' }, { (char) 0xE7, 'e' },
	{ (char) 0xE8, 'e' }, { (char) 0xE9, 'e' }, { (char) 0xEA, 'e' },
	{ (char) 0xEB, 'i' }, { (char) 0xEC, 'i' }, { (char) 0xED, 'i' },
	{ (char) 0xEE, 'i' }, { (char) 0xEF, 'o' }, { (char) 0xF0, 'n' },
	{ (char) 0xF1, 'o' }, { (char) 0xF2, 'o' }, { (char) 0xF3, 'o' },
	{ (char) 0xF4, 'o' }, { (char) 0xF5, 'o' }, { (char) 0xF6, 'o' },
	{ (char) 0xF8, 'o' }, { (char) 0xF9, 'u' }, { (char) 0xFA, 'u' },
	{ (char) 0xFB, 'u' }, { (char) 0xFC, 'u' }, { (char) 0xFD, 'y' },
	{ 0, 0 }
};
