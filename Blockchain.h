#pragma once
#include <QObject>
#include "Block.h"

namespace SBC
{
    class Block;

    class BlockChain : public QObject
    {
        Q_OBJECT
    public:
        BlockChain(int pDifficulty, int pMiningReward);
        explicit BlockChain(QObject *parent=0);

        void addGenesisBlock();
        Block* getLatestBlock();

        bool isChainValid();

        int getDifficulty() const;
        void setDifficulty(int value);

        int getMiningReward() const;
        void setMiningReward(int value);

        void minePendingTransactions(const QString & miningRewardAddress);
        void addToPendingTransaction(Transaction* tx);
        double getBalanceOfAddress(const QString & address);

        std::vector<Block*> chain;
        std::vector<Transaction*> pendingTransactions;

    private:
        int mDifficulty;
        int mMiningReward;

        explicit BlockChain(const BlockChain& rhs) = delete;

        BlockChain& operator = (const BlockChain& rhs) = delete;
    };
}



