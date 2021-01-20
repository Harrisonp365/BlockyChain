#include "Block.h"
#include <iostream>

namespace SBC
{

    Block::Block(qint64 pTimestamp, const std::vector<Transaction*> &pTransactions, QByteArray pPreviousHash) :
        mTimestamp(pTimestamp),
        transactions(pTransactions),
        mPreviousHash(pPreviousHash),
        mHash(calcHash()),
        mNonce(0)
    {
    }

    Block::Block(qint64 pTimestamp, const std::vector<Transaction*> &pTransactions, QByteArray pPreviousHash, QByteArray pHash, uint pNonce) :
        mTimestamp(pTimestamp),
        transactions(pTransactions),
        mPreviousHash(pPreviousHash),
        mHash(pHash),
        mNonce(pNonce)
    {
    }

    Block::Block(QObject *parent) : QObject(parent)
    {
    }

    qint64 Block::getGensisBlockTimestamp()
    {
        return QDateTime::fromString(QString("2018-01-13T02:17:00"), Qt::ISODate).toSecsSinceEpoch();
    }

    qint64 Block::getTimestamp() const
    {
        return mTimestamp;
    }

    void Block::setTimestamp(const qint64 &value)
    {
        mTimestamp = value;
    }

    uint Block::getNonce() const
    {
        return mNonce;
    }

    void Block::setNonce(const uint &value)
    {
        mNonce = value;
    }

    void Block::addTransaction(Transaction *value)
    {
        transactions.push_back(value);
    }

    QByteArray Block::getPreviousHash() const
    {
        return mPreviousHash;
    }

    void Block::setPreviousHash(const QByteArray &value)
    {
        mPreviousHash = value;
    }

    QByteArray Block::getHash() const
    {
        return mHash;
    }

    void Block::setHash(const QByteArray &value)
    {
        mHash = value;
    }

    QByteArray Block::calcHash()
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        QByteArray inputByteArray;
        QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);

        bool mutated = false;
        dataStream << mPreviousHash
                   << mTimestamp
                   << BlockMerkleRoot(&mutated)
                   << mNonce;
        crypto.addData(inputByteArray);

        return crypto.result();
    }

    void Block::mineBlock(int difficulty)
    {
        QByteArray zeros ;
        zeros.append(difficulty, 0); // The more 0's the more difficult

        while (mHash.left(difficulty) != zeros)
        {
            mNonce++;
            mHash = calcHash();
        }

        std::cout << "\nBLOCK MINED NONCE: " << mNonce << std::endl;
        std::cout << "Previous Block HASH: " << getPreviousHash().toHex().toStdString() << std::endl ;
        std::cout << "BLOCK MINED HASH: " << getHash().toHex().toStdString() << std::endl << std::endl;
    }

    QByteArray Block::getGenesisHash()
    {
        QCryptographicHash crypto(QCryptographicHash::Sha3_256);
        QByteArray inputByteArray;
        QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
        dataStream << "noHash" << 1010101010 << "from:To:Amount:10000" << 0;
        crypto.addData(inputByteArray);

        return crypto.result();
    }

    void Block::MerkleComputation(const std::vector<QByteArray> &leaves, QByteArray* proot, bool* pmutated)
    {
        bool mutated = false;
        // count is the number of leaves processed so far.
        uint32_t count = 0;
        // inner is an array of eagerly computed subtree hashes, indexed by tree
        // level (0 being the leaves).
        // For example, when count is 25 (11001 in binary), inner[4] is the hash of
        // the first 16 leaves, inner[3] of the next 8 leaves, and inner[0] equal to
        // the last leaf. The other inner entries are undefined.
        QByteArray inner[32];

        QCryptographicHash crypto(QCryptographicHash::Sha256);

        // Which position in inner is a hash that depends on the matching leaf.
        // First process all leaves into 'inner' values.
        while (count < leaves.size())
        {
            QByteArray h = leaves[count];
            count++;
            int level;

            // For each of the lower bits in count that are 0, do 1 step. Each
            // corresponds to an inner value that existed before processing the
            // current leaf, and each needs a hash to combine it.
            crypto.reset();
            for (level = 0; !(count & (((uint32_t)1) << level)); level++)
            {
                mutated |= (inner[level] == h); // mutated = mutated || (inner[level] == h)
                QByteArray inputByteArray;
                QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
                dataStream << inner[level] << h;
                crypto.addData(inputByteArray);
                h = crypto.result();
            }

            // Store the resulting hash at inner position level.
            inner[level] = h;
        }

        // Do a final 'sweep' over the rightmost branch of the tree to process
        // odd levels, and reduce everything to a single top value.
        // Level is the level (counted from the bottom) up to which we've sweeped.
        int level = 0;

        // As long as bit number level in count is zero, skip it. It means there
        // is nothing left at this level.
        while (!(count & (((uint32_t)1) << level)))
        {
            level++;
        }

        QByteArray h = inner[level];

        crypto.reset();
        while (count != (((uint32_t)1) << level))
        {
            // If we reach this point, h is an inner value that is not the top.
            // We combine it with itself (Bitcoin's special rule for odd levels in
            // the tree) to produce a higher level one.
            QByteArray inputByteArray;
            QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
            dataStream << h << h;
            crypto.addData(inputByteArray);
            h = crypto.result();
            // Increment count to the value it would have if two entries at this
            // level had existed.
            count += (((uint32_t)1) << level);
            level++;
            // And propagate the result upwards accordingly.
            while (!(count & (((uint32_t)1) << level)))
            {
                QCryptographicHash crypto(QCryptographicHash::Sha256);
                QByteArray inputByteArray;
                QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
                dataStream << inner[level] << h;
                crypto.addData(inputByteArray);
                h = crypto.result();
                level++;
            }
        }
        // Return result.
        if (pmutated) *pmutated = mutated;
        if (proot) *proot = h;
    }

    QByteArray Block::ComputeMerkleRoot(const std::vector<QByteArray> &leaves, bool* mutated)
    {
        QByteArray hash;
        MerkleComputation(leaves, &hash, mutated);
        return hash;
    }

    QByteArray Block::BlockMerkleRoot(bool* mutated)
    {
        std::vector<QByteArray> leaves;
        leaves.resize(transactions.size());
        for (size_t s = 0; s < transactions.size(); s++) {
            leaves[s] = transactions[s]->getTxHash();
        }
        return ComputeMerkleRoot(leaves, mutated);
    }

}
