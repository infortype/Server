#ifndef SERVER_H
#define SERVER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/qsqlquery.h>
#include <QtSql/QSqlError>

#include <QString>
#include <QStringList>
#include <QVector>

class  Server
{
public:
    Server(const QString BaseName);
    ~Server();

    bool ConnectToBase();

    const QString GetDataBaseName() const{
        return dataBaseName;
    }

    enum Type{
        Text,
        Integer,
        Float,
        Date,
        Blob
    };


private:
    QSqlDatabase dataBase;
    QString dataBaseName;

};

class Table
{
public:
    Table(QString _TableName);
    virtual ~Table();

    //Others data
    enum Open{
        CreateSec,
        AddSec,
        GetSec,
        UpdateSec,
        DeleteSec

    };

    enum OrdeBy{
        Asc,
        Desc
    };

    //Rows
    void AddRow(const QString name, Server::Type type,
                const QString additionalRowInfo = "");
    const QString ReturnType(short type) const;
    QStringList GetRows() const;
    std::pair<QString, QString> GetRow(int r) const;
    QString GetRowName(const int row) const;

    //Create base
    bool Create() const;

    //Add
    bool Add(const QStringList& data);

    //Get
    virtual QVector<QStringList> GetAllElements() const;
    virtual QVector<QStringList> GetAllElements(const int rowOrder, const Table::OrdeBy orderBy) const;
    virtual QVector<QStringList> GetElement(const int row, const QString find) const;
    virtual QVector<QStringList> GetElement(const int row, const QString find, const int rowOrder, const Table::OrdeBy orderBy) const;
    const QByteArray GetImageFromTable(const int rowImage, const int idImage) const;

    //Update
    bool Update(const QStringList& data, const int rowId, const int idFind) const;

    //Delete item
    bool Delete(const int row, const QString find) const;

    //Delete Table
    bool DeleteTable() const;

    //Clear Data
    bool ClearData() const;

private:
    //Table data
    QString TableName;
    QVector<std::pair<QString, QString>> rows;
    QVector<QString> addRowInfo;
    const QVector<QString> Order = {"ASC", "DESC"};

    //Server
    Server *s;

    //Create
    QString PhraseCreate() const;

    //Add
    QString PhraseAdd() const;
    bool DetecIdAutomatic(int &indexAuto) const;

    //Create, Add and Get
    QString InitialPhrase(const Table::Open open) const;
    //QString CreateOtherPhrases(const Table::Open open) const;

    //Get
    QVector<QStringList> GetExec(const QString consultation, const int size, const QString msjGood, const QString msjBad) const;
    QString InitialPhraseOrder(const QString row, const QString order) const;
    QString InitialPhraseElement(const QString rowName, const QString find) const;
    QString InitialPhraseElementOrder(const QString rowName, const QString find, const QString rowOrderName, const QString Order) const;

    //Update
    QString PhraseUpdate(const QStringList& data) const;

};


#endif // SERVER_H
