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
    };
}



