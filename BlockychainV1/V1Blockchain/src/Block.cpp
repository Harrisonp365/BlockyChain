#include "Block.h"
#include "SHA256.h"
#include <iostream>
#include <cstdint>
#include <ctime>
#include <sstream>

using namespace std;

Block::Block(uint32_t indexIn, const string& dataIn) :
	mIndex(indexIn), mData(dataIn)
{
	mNonce = -1;
	mTime = time(nullptr); // current time
}

string Block::GetHash()
{
	return mHash;
}

void Block::mineBlock(uint32_t difficulty)
{
	char *cstr = new char[difficulty + 1];

	for (uint32_t i = 0; i < difficulty; ++i)
	{
		cstr[i] = '0';
	}
	cstr[difficulty] = '\0';

	string str(cstr);

	do 
	{
		mNonce++;
		mHash = mCalcHash();
	} while (mHash.substr(0, difficulty) != str);

	cout << "Block mined: " << mHash << "\n";
}

inline string Block::mCalcHash() const 
{
	stringstream ss;
	ss << mIndex << mTime << mData << mNonce << prevHash;

	return sha256(ss.str());
}