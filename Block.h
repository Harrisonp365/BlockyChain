#ifndef BLOCK_H
#define BLOCK_H

#pragma once
#include <QObject>
#include <QtCore>
#include <QCryptographicHash>
#include <QDebug>
//#include "transaction.h"

namespace SBC
{

class Transaction;

    class Block : public QObject
    {
        Q_OBJECT
    public:
        Block(qint64 pTimestamp, const std::vector<Transaction*>& pTransactions, QByteArray pPreviousHash = nullptr);
        Block(qint64 pTimestamp, const std::vector<Transaction*>& pTransactions, QByteArray pPreviousHash, QByteArray pHash, uint pNonce);
        explicit Block(QObject *parent = 0);

        qint64 getTimestamp() const;
        void setTimestamp(const qint64 &value);

        uint getNonce() const;
        void setNonce(const uint &value);

        QByteArray getPreviousHash() const;
        void setPreviousHash(const QByteArray &value);

        QByteArray getHash() const;
        void setHash(const QByteArray &value);

        std::vector<Transaction*> transactions;

        void addTransaction(Transaction* value);

        QByteArray calcHash();
        void mineBlock(int difficulty);

        static qint64 getGensisBlockTimestamp();
        static QByteArray getGenesisHash();

        void MerkleComputation(const std::vector<QByteArray> &leaves, QByteArray *proot, bool *pmutated);
        QByteArray ComputeMerkleRoot(const std::vector<QByteArray> &leaves, bool *mutated);
        QByteArray BlockMerkleRoot(bool* mutated);

    private:
        qint64 mTimestamp;
        QByteArray mPreviousHash;
        QByteArray mHash;
        uint mNonce;

        explicit Block(const Block& rhs) = delete;
        Block& operator= (const Block& rhs) = delete;
    };

}

#endif // BLOCK_H
