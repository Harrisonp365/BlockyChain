#include "Block.h"
#include <iostream>

namespace SBC
{

    Block::Block(qint64 pTimestamp, QByteArray pPreviousHash) :
        timestamp(pTimestamp),
        previousHash(pPreviousHash),
        hash(calcHash())
    {
    }

    Block::Block(qint64 pTimestamp, QByteArray pPreviousHash, QByteArray pHash) :
        timestamp(pTimestamp),
        previousHash(pPreviousHash),
        hash(pHash)
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
        return timestamp;
    }

    void Block::setTimestamp(const qint64 &value)
    {
        timestamp = value;
    }

    QByteArray Block::getPreviousHash() const
    {
        return previousHash;
    }

    void Block::setPreviousHash(const QByteArray &value)
    {
        previousHash = value;
    }

    QByteArray Block::getHash() const
    {
        return hash;
    }

    void Block::setHash(const QByteArray &value)
    {
        hash = value;
    }

    QByteArray Block::calcHash()
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        QByteArray inputByteArray;
        QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);

        //bool mutated = false;
        dataStream << previousHash
                   << timestamp;
                   //<< BlockMerkleRoot(&mutated)
                   //<< nonce; // This is for later when I impliment proof of work
        crypto.addData(inputByteArray);

        return crypto.result();
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

}
