#ifndef BLOCK_H
#define BLOCK_H

#pragma once
#include <QObject>
#include <QtCore>
#include <QCryptographicHash>
#include <QDebug>

namespace SBC
{
    class Block : public QObject
    {
        Q_OBJECT
    public:
        Block(qint64 pTimestamp, QByteArray pPreviousHash = nullptr);
        Block(qint64 pTimestamp, QByteArray pPreviousHash, QByteArray pHash);
        explicit Block(QObject *parent = 0);

        qint64 getTimestamp() const;
        void setTimestamp(const qint64 &value);

        QByteArray getPreviousHash() const;
        void setPreviousHash(const QByteArray &value);

        QByteArray getHash() const;
        void setHash(const QByteArray &value);

        QByteArray calcHash();

        static qint64 getGensisBlockTimestamp();
        static QByteArray getGenesisHash();

    private:
        qint64 timestamp;
        QByteArray previousHash;
        QByteArray hash;

        explicit Block(const Block& rhs) = delete;
        Block& operator= (const Block& rhs) = delete;
    };

}

#endif // BLOCK_H
