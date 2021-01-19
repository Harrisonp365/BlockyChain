#pragma once
#include <cstdint>
#include <vector>
#include "Block.h"

using namespace std;

class Blockchain {
public:
	Blockchain();

	void AddBlock(Block newB);

private:
	uint32_t mDifficulty;
	vector<Block> mChain;
	Block getLastBlock() const;
};
