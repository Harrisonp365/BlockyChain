#include "Blockchain.h"

using namespace std;

Blockchain::Blockchain() 
{
	mChain.emplace_back(Block(0, "Genesis Block"));

	mDifficulty = 5;
}

void Blockchain::AddBlock(Block newB)
{
	newB.prevHash = getLastBlock().GetHash();
	newB.mineBlock(mDifficulty);
	mChain.push_back(newB);
}

Block Blockchain::getLastBlock() const
{
	return mChain.back();
}