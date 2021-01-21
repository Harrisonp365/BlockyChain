#include <iostream>
#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDir>
#include "Blockchain.h"

using namespace SBC;

void chainToJson(BlockChain* hCoin)
{
    QJsonObject objRoot; // root object

    QJsonArray blocks; //2 blocks

    for(auto i = 0; i < hCoin->chain.size(); i++)
    {
        QJsonObject block;
        block["BlockIndex"] = QString::number(i);
        block["PreviousHash"] = QString(hCoin->chain[i]->getPreviousHash().toHex());
        block["BlockHash"] = QString(hCoin->chain[i]->getHash().toHex());
        block["Nonce"] = QString::number(hCoin->chain[i]->getNonce());
        block["TimeStamp"] = QString::number(hCoin->chain[i]->getTimestamp());

        QJsonArray transactions;
        unsigned int j = 0;
        for(const auto& tx : hCoin->chain[i]->transactions)
        {
            QJsonObject transaction;
            transaction["TxIndex"] = QString::number(j);
            transaction["from"] = tx->getFrom();
            transaction["to"] = tx->getTo();
            transaction["amount"] = tx->getAmount();
            transaction["TxHash"] = QString(tx->getTxHash().toHex());
            transactions.append(transaction);
            j++;
        }
        block["Transactions"] = transactions;
        blocks.append(block);
    }
    objRoot["BlockyChain"] = blocks;

    std::cout << "\n\nBlockyChain: \n" << QJsonDocument(objRoot).toJson(QJsonDocument::Indented).toStdString() <<std::endl;

    // save JSON to disk

    auto BlockyChainToJsonFilePath = QDir::currentPath() + "/BlockyChainJson.json";
    QFile BlockyChainJsonfile(BlockyChainToJsonFilePath);
    if(!BlockyChainJsonfile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    //empty old file if already exists
    BlockyChainJsonfile.resize(0);
    BlockyChainJsonfile.write(QJsonDocument(objRoot).toJson(QJsonDocument::Indented));
    std::cout << "\nBlockyChainToJson path is:" + BlockyChainToJsonFilePath.toStdString() << std::endl;

    BlockyChainJsonfile.flush();
    BlockyChainJsonfile.close();
}

void chainToStream(QDataStream& out, BlockChain* hCoin)
{
    out << hCoin->getDifficulty();
    out << hCoin->getMiningReward();

    //block count
    long long blockCount = hCoin->chain.size();
    out << blockCount;

    // out the blocks, first out the tx count, then content of block header, then loop each tx
    for(const auto& bx : hCoin->chain)
    {
        //tx count and header
        unsigned int txCount = bx->transactions.size();
        out << txCount;
        out << bx->getTimestamp();
        out << bx->getNonce();
        QString endBh = "endBlockHeader";
        out << endBh;

        //tx info
        for(const auto& tx : bx->transactions)
        {
            out << tx->getFrom();
            out << tx->getTo();
            out << tx->getAmount();
            QString endTx = "endTx";
            out << endTx;
        }
        QString endBk = "endBlock";
        out << endBk;
    }
}

void chainFromStream(QDataStream& in, BlockChain*& hCoin)
{
    int difficulty;
    int miningReward;

    in >> difficulty;
    in >> miningReward;
    hCoin = new BlockChain(difficulty, miningReward);

    long long blockCount;
    in >> blockCount;

    std::vector<Block*> blocks;

    for(auto i = 0; i < blockCount; i++)
    {
        unsigned int txCount;
        in >> txCount;

        qint64 timestamp;
        uint nonce;
        QString endBh;
        QString endBk;
        in >> timestamp;
        in >> nonce;
        in >> endBh;

        std::vector<Transaction*> txs;

        for(unsigned int j = 0; j < txCount; j++)
        {
            QString from;
            QString to;
            double amount;
            QString endTx;
            in >> from;
            in >> to;
            in >> amount;
            in >> endTx;
            if(i != 0)
                txs.push_back(new Transaction(from, to, amount));
        }
        in >> endBk;
        if(i != 0)
            hCoin->chain.push_back(new Block(timestamp, txs, hCoin->getLatestBlock()->getHash()));
    }

    //mine all blocks except for genesis(first block)
    for(auto k = 1; k < blockCount; k++)
        hCoin->chain[k]->mineBlock(hCoin->getDifficulty());
}

int initMain()
{
    std::cout << "Welcome to BlockyChainV2..." << std::endl;

    BlockChain* savedCoin = new BlockChain(1, 100); // difficulty and mining reward
    auto readChainFromFilePath = QDir::currentPath() + "/BlockyChainBinary.json";
    QFile readChainToFile(readChainFromFilePath);
    if(readChainToFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QDataStream in (&readChainToFile);
        in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        chainFromStream(in, savedCoin);
    }

    if(savedCoin->chain.size() > 1)
    {
        std::cout << "there is a saved chain, do you want to retrieve this chain?" << std::endl;
        std::cout << "1:yes, 2:no" << std::endl;
        unsigned int ans;
        std::cin >> ans;

        if(ans == 1)
        {
            Transaction* tx1 = new Transaction("fromAddress", "toAddress", 30.0);
            Transaction* tx2 = new Transaction("HarryAddress", "LeeAddress", 100);

            savedCoin->addToPendingTransactions(tx1);
            savedCoin->addToPendingTransactions(tx2);

            std::cout << "\nStarting Miner...\n";
            std::cout << "\nEnter address of miner: \n";

            std::string mnrAddress;
            std::cin >> mnrAddress;
            QString minerAddress = QString::fromStdString(mnrAddress);
            savedCoin->minePendingTransactions(minerAddress);

            std::cout << "current balance of Miner: " + minerAddress.toStdString() + " is:\n " <<
                         savedCoin->getBalanceOfAddress(minerAddress) << std::endl;

            Transaction* tx3 = new Transaction("fromAddress","toAddress",10.0);
            Transaction* tx4 = new Transaction("HarryAddress","LeeAddress",150);
            Transaction* tx5 = new Transaction("LeeAddress","HarryAddress",15.50);
            Transaction* tx6 = new Transaction(minerAddress,"LeeAddress",18.50);
            Transaction* tx7 = new Transaction("fromAddress", "HarryAddress",200.0);

            savedCoin->addToPendingTransactions(tx3);
            savedCoin->addToPendingTransactions(tx4);
            savedCoin->addToPendingTransactions(tx5);
            savedCoin->addToPendingTransactions(tx6);
            savedCoin->addToPendingTransactions(tx7);

            std::cout << "\nStarting to mine the next block...\n";
            std::cout << "\nEnter address of miner: \n";
            std::cin >> mnrAddress;
            minerAddress = QString::fromStdString(mnrAddress);

            savedCoin->minePendingTransactions(minerAddress);

            std::cout << "current balance of Miner: " + minerAddress.toStdString() + " is:\n" <<
                         savedCoin->getBalanceOfAddress(minerAddress) << std::endl;

            //convert blockchain to Json then print
            chainToJson(savedCoin);

            auto isChainValid = savedCoin->isChainValid() ? "true" : "false";
            std::cout << "Blockcahin vaild? " << isChainValid << std::endl;

            auto saveChainTofilePath = QDir::currentPath() + "/BlockyChainBinary.json";
            QFile saveChainToFile(saveChainTofilePath);
            if(!saveChainToFile.open(QIODevice::WriteOnly | QIODevice::Text))
                return 0;

            QDataStream out (&saveChainToFile);
            out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

            //empty old file if exists
            saveChainToFile.resize(0);

            chainToStream(out, savedCoin);

            saveChainToFile.flush();
            saveChainToFile.close();
            return 0;
        }
    }
    else
    {
        std::cout << "Enter a block difficulty number between 1-3: " << std::endl;
        uint difficulty = 1;
        std::cin >> difficulty;

        if(difficulty > 3 || difficulty < 1)
        {
            difficulty = 2; //default
        }

        std::cout << "Enter mining reward amount between 1-1000: " << std::endl;
        int rewards = 1;
        std::cin >> rewards;
        if(rewards > 1000 || rewards < 1)
        {
            rewards = 50; //default
        }
        BlockChain* hCoin = new BlockChain(difficulty, rewards);

        std::cout << "Mining Block 1..." << std::endl;

        Transaction* tx1 = new Transaction("fromAddress", "toAddress", 30.0);
        Transaction* tx2 = new Transaction("HarryAddress", "LeeAddress", 100);

        hCoin->addToPendingTransactions(tx1);
        hCoin->addToPendingTransactions(tx2);

        std::cout << "\nStarting miner...\n";
        std::cout << "\nEnter address of miner: ";

        std::string mnrAddress;
        std::cin >> mnrAddress;
        QString minerAddress = QString::fromStdString(mnrAddress);
        hCoin->minePendingTransactions(minerAddress);

        std::cout << "current balance of Miner: " + minerAddress.toStdString() + "is:\n" <<
                     hCoin->getBalanceOfAddress(minerAddress) << std::endl;

        Transaction* tx3 = new Transaction("fromAddress","toAddress",10.0);
        Transaction* tx4 = new Transaction("HarryAddress","LeeAddress",150);
        Transaction* tx5 = new Transaction("LeeAddress","HarryAddress",15.50);
        Transaction* tx6 = new Transaction(minerAddress,"LeeAddress",18.50);
        Transaction* tx7 = new Transaction("fromAddress", "HarryAddress",200.0);

        hCoin->addToPendingTransactions(tx3);
        hCoin->addToPendingTransactions(tx4);
        hCoin->addToPendingTransactions(tx5);
        hCoin->addToPendingTransactions(tx6);
        hCoin->addToPendingTransactions(tx7);

        std::cout << "\nStarting to mine the next block...\n";
        std::cout << "\nEnter address of miner:\n";
        std::cin >> mnrAddress;
        minerAddress = QString::fromStdString(mnrAddress);
        hCoin->minePendingTransactions(minerAddress);

        std::cout << "current balance of Miner: " + minerAddress.toStdString() + "  is:\n" <<
                     hCoin->getBalanceOfAddress(minerAddress) << std::endl;

        //convert to json object and print
        chainToJson(hCoin);

        auto isChainValid = hCoin->isChainValid() ? "true" : "false";
        std::cout << "Blockchain valid?" << isChainValid << std::endl;

        auto saveChainToFilePath = QDir::currentPath() + "/BlockyChainBinary.json";
        QFile saveChainToFile(saveChainToFilePath);
        if(!saveChainToFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return 0;

        QDataStream out (&saveChainToFile);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        saveChainToFile.resize(0);
        chainToStream(out, hCoin);
        saveChainToFile.flush();
        saveChainToFile.close();
    }
    return 0;
}

int main(int argc, char* argv[])
{
    try
    {
        initMain();
    }
    catch (std::exception &e)
    {
        std::cout << "Error" << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Error <unkown>" << std::endl;
    }
    return 0;
}
