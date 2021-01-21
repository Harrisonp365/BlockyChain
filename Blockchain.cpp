#include "Blockchain.h"
#include <iostream>

namespace SBC
{
    BlockChain::BlockChain(QObject *parent) : QObject(parent)
    {
    }

    BlockChain::BlockChain(int pDifficulty, int pMiningReward) : mDifficulty(pDifficulty), mMiningReward(pMiningReward)
    {
        addGenesisBlock();
    }

    int BlockChain::getDifficulty() const
    {
        return mDifficulty;
    }

    void BlockChain::setDifficulty(int value)
    {
        mDifficulty = value;
    }

    int BlockChain::getMiningReward() const
    {
        return mMiningReward;
    }

    void BlockChain::setMiningReward(int value)
    {
        mMiningReward = value;
    }

    void BlockChain::minePendingTransactions(const QString &miningRewardAddress)
    {
        auto block = new Block(QDateTime::currentSecsSinceEpoch(), pendingTransactions, getLatestBlock()->getHash());

        block->mineBlock(mDifficulty);
        std::cout << "Block successfully mined...\n";

        //add block to chain
        chain.push_back(block);
        pendingTransactions.clear();

        auto tx = new Transaction("", miningRewardAddress, getMiningReward());
        pendingTransactions.push_back(tx);
    }

    void BlockChain::addToPendingTransactions(Transaction *tx)
    {
        pendingTransactions.push_back(tx);
    }

    double BlockChain::getBalanceOfAddress(const QString &address)
    {
        double balance = 0.0;

        for(const auto& block : chain)
        {
            for(const auto& tx : block->transactions)
            {
                if(tx->getFrom() == address)
                    balance -= tx->getAmount();

                if(tx->getTo() == address)
                    balance += tx->getAmount();
            }
        }
        return balance;
    }

    void BlockChain::addGenesisBlock()
    {
        if(chain.size() > 0)
            return;

        std::vector<Transaction*> txs;
        txs.push_back(new Transaction("FromGenesisBlock", "HarryAddress", 10000.0));

        Block* genesisBlock = new Block(Block::getGensisBlockTimestamp(), txs);
        genesisBlock->setHash(Block::getGenesisHash());

        chain.push_back(genesisBlock);
    }

    Block* BlockChain::getLatestBlock()
    {
        return chain[chain.size() - 1];
    }

    bool BlockChain::isChainValid()
    {
        for(auto i = 1; i < chain.size(); i++)
        {
            auto currentBlock = chain[i];
            auto previousBlock = chain[i - 1];

            if(currentBlock->getHash() != currentBlock->calcHash())
                return false;
            if(currentBlock->getPreviousHash() != previousBlock->getHash())
                return false;
        }
        return true;
    }
}


