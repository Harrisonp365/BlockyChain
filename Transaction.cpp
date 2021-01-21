#include "Transaction.h"

namespace SBC
{
    Transaction::Transaction(QString pFrom, QString pTo, double pAmount):
        mFrom(pFrom),
        mTo(pTo),
        mAmount(pAmount)
    {
        mHash = calculateHash();
    }

    Transaction::Transaction(QString pFrom, QString pTo, double pAmount, QByteArray pHash):
        mFrom(pFrom),
        mTo(pTo),
        mAmount(pAmount),
        mHash(pHash)
    {
    }

    Transaction::Transaction(QObject *parent): QObject(parent)
    {
    }

    QString Transaction::getFrom() const
    {
        return mFrom;
    }

    void Transaction::setFrom(const QString &value)
    {
        mFrom = value;
    }

    QString Transaction::getTo() const
    {
        return mTo;
    }

    void Transaction::setTo(const QString &value)
    {
        mTo = value;
    }

    double Transaction::getAmount() const
    {
        return mAmount;
    }

    void Transaction::setAmount(double value)
    {
        mAmount = value;
    }

    QByteArray Transaction::calculateHash()
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        QByteArray inputByteArray;
        QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);

        dataStream << mFrom
                   << mTo
                   << mAmount;

        crypto.addData(inputByteArray);


        return crypto.result();
    }

    QByteArray Transaction::getTxHash() const
    {
        return mHash;
    }

    void Transaction::setTxHash(const QByteArray &value)
    {
        mHash = value;
    }
}

