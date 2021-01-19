#pragma once
#include <cstdint>
#include <iostream>

using namespace std;

class Block {
public:
	string prevHash; //Hash of last block
	Block(uint32_t indexIn, const string& dataIn);
	string GetHash();

	void mineBlock(uint32_t difficulty);

private:
	uint32_t mIndex; //_nIndex
	int64_t mNonce; // _nNounce
	string mData; // _sData
	string mHash; // _sHash
	time_t mTime; // _tTime

	string mCalcHash() const;
};