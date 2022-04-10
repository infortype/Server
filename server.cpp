#include "server.h"
#include <QDebug>

Server::Server(const QString BaseName)
{

     if(BaseName.isEmpty())
     {

         qDebug().noquote() << "[-] The name of the database cannot be empty. {server.cpp}[Server]";

     }else
     {

         qDebug().noquote() << QString("[+] The name of the database is '%1'.").arg(BaseName);
         dataBaseName = BaseName;

     }

    return;

}

Server::~Server()
{

}

bool Server::ConnectToBase()
{

    bool created = true;

    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(dataBaseName);

     if (!dataBase.open())
     {

         qDebug().noquote() <<QString("[-] A database '%1' was not conected. {server.cpp}[ConnectToBase]").arg(dataBaseName);
         created = false;

     }else
     {

         qDebug().noquote() <<QString("[+] A database '%1' was conected.").arg(dataBaseName);

     }

    return created;

}

Table::Table(QString _TableName)
{

     if(_TableName.isEmpty())
     {

         qDebug().noquote() <<QString("[-] The name of the table can´t be empty. {server.cpp}[Table]");
         return;

     }

    TableName = _TableName;

}

Table::~Table()
{

}

void Table::AddRow(const QString name, Server::Type type, const QString additionalRowInfo)
{

     if(name.isEmpty())
     {

         qDebug().noquote() <<QString("[-] The name of the row can´t be empty. {server.cpp}[AddRow]");

     }else
     {

         const QString typeStr = ReturnType(type);

         std::pair<QString, QString> row;
         row.first = name;
         row.second = typeStr;

         rows.push_back(row);
         addRowInfo.push_back(additionalRowInfo);


     }

    return;

}

const QString Table::ReturnType(short type) const
{

    QString typeString;

     switch (type)
     {
     case 0:
         typeString = "TEXT";
         break;
     case 1:
         typeString = "INTEGER";
         break;
     case 2:
         typeString = "FLOAT";
         break;
     case 3:
         typeString = "DATE";
         break;

     case 4:
         typeString = "BLOB";
         break;

     default:
         typeString = "TEXT";
         break;
     }

     return typeString;

}

QStringList Table::GetRows() const
{

    int size = rows.size();
    QStringList list;

     for(int i = 0; i <= size - 1; i ++)
     {

         QString text = QString("Row: %1 - Type: %2").arg(rows.at(i).first, rows.at(i).second);
         list << text;

     }

     return list;

}

std::pair<QString, QString> Table::GetRow(int r) const
{

    std::pair<QString, QString> row;
    row.first = rows.at(r).first;
    row.second = rows.at(r).second;

    return row;

}

QString Table::GetRowName(const int row) const
{

    QString name;
    const int size = rows.size() - 1;

     for(int i = 0; i <= size; i++)
     {

          if(i == row)
          {

               name = rows.at(i).first;

          }

     }

    return name;

}

bool Table::Create() const
{
    bool created = true;
    QString consultation = QString("%1%2").arg(InitialPhrase(CreateSec), PhraseCreate());
    QSqlQuery ToCreate;
    ToCreate.prepare(consultation);

     if(!ToCreate.exec())
     {

         qDebug().noquote() <<QString("[-] A table '%1' was not created. {server.cpp}[Create] Info: [%2]").arg(TableName, consultation);
         created = false;

     }else
     {

         qDebug().noquote() <<QString("[+] A table '%1' was created. {server.cpp}[Create]").arg(TableName);

     }

    return created;

}

bool Table::Add(const QStringList &data)
{

    bool add = true;
    QSqlQuery AddQuery;
    int indexAuto;
    bool idAutomatic = DetecIdAutomatic(indexAuto);

    QString consultation = QString("%1%2").arg(InitialPhrase(AddSec), PhraseAdd());
    AddQuery.prepare(consultation);

     for(int i = 0; i <= data.size() - 1; i++)
     {
          if(idAutomatic && indexAuto == i) continue;

         AddQuery.addBindValue(data.at(i));

     }

     if(AddQuery.exec())
     {

         qDebug().noquote() <<QString("[+] Data was put into the table '%1'. {server.cpp}[Add]").arg(TableName);

     }else
     {

         qDebug().noquote() <<QString("[-] Data was not put into the table '%1'. {server.cpp}[Add] Info: [%2]").arg(TableName, consultation);
         add = false;

     }

    return add;

}

QVector<QStringList> Table::GetAllElements() const
{

    const int allRows = rows.size() - 1;
    const QString consultation = InitialPhrase(GetSec);
    const QString msjGood = QString("Data was output from the table '%1'. {server.cpp}[GetAllElements]").arg(TableName);
    const QString msjBad = QString("Data was not output from the table '%1'. {server.cpp}[GetAllElements]").arg(TableName);

    return GetExec(consultation, allRows, msjGood, msjBad);

}

QVector<QStringList> Table::GetAllElements(const int rowOrder, const OrdeBy orderBy) const
{

    const QString order = GetRowName(rowOrder);
    const QString orderFunc = order.at(orderBy);
    const int allRows = rows.size() - 1;
    const QString consultation = InitialPhraseOrder(order,orderFunc);
    const QString msjGood = QString("Data was output from the table '%1'. {server.cpp}[GetAllElements(2)]").arg(TableName);
    const QString msjBad = QString("Data was not output from the table '%1'. {server.cpp}[GetAllElements(2)]").arg(TableName);

    return GetExec(consultation, allRows, msjGood, msjBad);

}

QVector<QStringList> Table::GetElement(const int row, const QString find) const
{

    const QString rowName = GetRowName(row);
    const QString consultation = InitialPhraseElement(rowName, find);
    const int allRows = rows.size() - 1;
    const QString msjGood = QString("Data was output from the table '%1'. {server.cpp}[GetElement]").arg(TableName);
    const QString msjBad = QString("Data was not output from the table '%1'. {server.cpp}[GetElement]").arg(TableName);

    return  GetExec(consultation, allRows, msjGood, msjBad);

}

QVector<QStringList> Table::GetElement(const int row, const QString find, const int rowOrder, const OrdeBy orderBy) const
{

    const QString rowName = GetRowName(row);
    const QString rowOrderName = GetRowName(rowOrder);
    const QString consultation = InitialPhraseElementOrder(rowName, find, rowOrderName, Order.at(orderBy));
    const int allRows = rows.size() - 1;
    const QString msjGood = QString("Data was output from the table '%1'. {server.cpp}[GetElement(2)]").arg(TableName);
    const QString msjBad = QString("Data was not output from the table '%1'. {server.cpp}[GetElement(2)]").arg(TableName);

    return GetExec(consultation, allRows,msjGood,msjBad);

}

const QByteArray Table::GetImageFromTable(const int rowImage, const int idImage) const
{

    const QString ImageRowName = GetRowName(rowImage);
    const QString consultation = QString("SELECT %1 FROM %2 WHERE id = %3").arg(ImageRowName, TableName, QString::number(idImage));
    QSqlQuery get;
    get.prepare(consultation);

     if(get.exec())
     {

         qDebug().noquote() <<QString("[+] Image was output from the table '%1'. {server.cpp}[GetImageFromTable]").arg(TableName);

     }else
     {

         qDebug().noquote() <<QString("[-] Image was not output from the table '%1'. {server.cpp}[GetImageFromTable] "
                                      "info: ").arg(TableName) << get.lastError();

     }

    get.next();
    const QByteArray byte = get.value(0).toByteArray();

    return byte;

}

bool Table::Update(const QStringList &data, const int rowId, const int idFind) const
{

    bool upd = true;
    const QString RowName = GetRowName(rowId);
    QSqlQuery UpdateQuery;
    const QString consultation = QString("%1 %2 WHERE %3 = %4").arg(InitialPhrase(UpdateSec), PhraseUpdate(data), RowName, QString::number(idFind));
    UpdateQuery.prepare(consultation);

     if(UpdateQuery.exec())
     {

         qDebug().noquote() <<QString("[+] Data was update in the table '%1'. {server.cpp}[Update]").arg(TableName);

     }else
     {

         qDebug().noquote() <<QString("[-] Data was not update in the table '%1'. {server.cpp}[Update] Info: [%2]").arg(TableName, consultation);
         qDebug().noquote() << UpdateQuery.lastError();
         upd = false;

     }

    return upd;

}

bool Table::Delete(const int row, const QString find) const
{

    bool del = true;
    const QString RowName = GetRowName(row);
    QSqlQuery DelQuery;
    const QString consultation = QString("%1 WHERE %2 = '%3'").arg(InitialPhrase(DeleteSec), RowName, find);
    DelQuery.prepare(consultation);

    if(DelQuery.exec())
    {

        qDebug().noquote() <<QString("[+] Data was eliminated from the table '%1'. {server.cpp}[Delete]").arg(TableName);

    }else
    {

        qDebug().noquote() <<QString("[-] Data was not eliminated from the table '%1'. {server.cpp}[Delete] Info: [%2]").arg(TableName, consultation);
        qDebug().noquote() << DelQuery.lastError();
        del = false;

    }

   return del;

}

bool Table::DeleteTable() const
{

    bool del = true;
    QSqlQuery DelQuery;
    const QString consultation = QString("DROP TABLE %1").arg(TableName);
    DelQuery.prepare(consultation);

    if(DelQuery.exec())
    {

        qDebug().noquote() <<QString("[+] Table was eliminated '%1'. {server.cpp}[DeleteTable]").arg(TableName);

    }else
    {

        qDebug().noquote() <<QString("[-] Table was not eliminated table '%1'. {server.cpp}[DeleteTable] Info: [%2]").arg(TableName, consultation);
        qDebug().noquote() << DelQuery.lastError();
        del = false;

    }

   return del;

}

bool Table::ClearData() const
{

    bool clear = true;
    QSqlQuery ClearQuery;
    const QString consultation = QString("DELETE FROM Tabla1;")/*.arg(TableName)*/;
    ClearQuery.prepare(consultation);
    qDebug().noquote() << consultation;

    if(ClearQuery.exec())
    {

        qDebug().noquote() <<QString("[+] Table was Cleared '%1'. {server.cpp}[ClearData]").arg(TableName);

    }else
    {

        qDebug().noquote() <<QString("[-] Table was not cleared '%1'. {server.cpp}[ClearData] Info: [%2]").arg(TableName, consultation);
        qDebug().noquote() << ClearQuery.lastError();
        clear = false;

    }

   return clear;

}

QString Table::InitialPhrase(const Open open) const
{

    QString str;

     switch (open)
     {

     case CreateSec:
         str = QString("CREATE TABLE IF NOT EXISTS %1 (").arg(TableName);
         break;

     case AddSec:
         str = QString("INSERT INTO %1 (").arg(TableName);
         break;

     case GetSec:
         str = QString("SELECT * FROM %1").arg(TableName);
         break;

     case UpdateSec:
         str = QString("UPDATE %1 SET").arg(TableName);
         break;

     default:
         str = QString("DELETE FROM %1").arg(TableName);
         break;

     }

    return str;

}

QString Table::PhraseCreate() const
{

    const int size = rows.size() - 1;
    QString FinalPhrase;

     for(int i = 0; i <= size; i++)
     {

         if(i == size)
         {

             QString string = QString("%1 %2 %3);").arg(rows.at(i).first, rows.at(i).second, addRowInfo.at(i));
             FinalPhrase.append(string);

         }else
         {

              QString string = QString("%1 %2 %3,").arg(rows.at(i).first, rows.at(i).second, addRowInfo.at(i));
              FinalPhrase.append(string);

         }

     }

     return FinalPhrase;

}

QString Table::PhraseAdd() const
{

    const int size = rows.size() - 1;
    QString FinalPhrase;

    int indexAuto;
    bool idAutomatic = DetecIdAutomatic(indexAuto);

     for(int i = 0; i <= size; i++)
     {

          if(idAutomatic && i == indexAuto)
          {

               if(i!= size) continue;

              FinalPhrase.remove((FinalPhrase.size() - 1),1);
              FinalPhrase.append(")");

          }

          if(i == size)
          {

              QString string = QString("%1)").arg(rows.at(i).first);
              FinalPhrase.append(string);

          }else
          {

              QString string = QString("%1,").arg(rows.at(i).first);
              FinalPhrase.append(string);

          }

     }

    FinalPhrase.append(" VALUES(");

     for(int i = 0; i <= size; i++)
     {

          if(idAutomatic && i == indexAuto)
          {

               if(i!= size) continue;
              FinalPhrase.remove((FinalPhrase.size() - 1),1);
              FinalPhrase.append(")");

          }

          if(i == size)
          {

              QString string = QString("?)");
              FinalPhrase.append(string);

          }else
          {

              QString string = QString("?,");
              FinalPhrase.append(string);

          }

     }

    return FinalPhrase;

}

QString Table::PhraseUpdate(const QStringList &data) const
{

    const int size = rows.size() - 1;
    QString FinalPhrase;

    int indexAuto;
    bool idAutomatic = DetecIdAutomatic(indexAuto);

    for(int i = 0; i <= size; i++)
    {

         if(idAutomatic && i == indexAuto)
         {

              if(i!= size) continue;

             FinalPhrase.remove((FinalPhrase.size() - 1),1);
             FinalPhrase.append(")");

         }

         if(i == size)
         {

             QString string;

              if(rows.at(i).second == "FLOAT")
              {

                  string = QString("%1 = %2").arg(rows.at(i).first, data.at(i));

              }else
              {

                  string = QString("%1 = '%2'").arg(rows.at(i).first, data.at(i));

              }

             FinalPhrase.append(string);

         }else
         {

             QString string;

              if(rows.at(i).second == "FLOAT")
              {

                  string = QString("%1 = '%2', ").arg(rows.at(i).first, data.at(i));

              }else
              {

                  string = QString("%1 = '%2', ").arg(rows.at(i).first, data.at(i));

              }

             FinalPhrase.append(string);

         }

    }

   return FinalPhrase;

}

bool Table::DetecIdAutomatic(int& indexAuto) const
{

    bool idAutomatic = false;
    const int size = addRowInfo.size() - 1;

     for(int i = 0; i <= size; i++)
     {

          if(addRowInfo.at(i).contains("AUTOINCREMENT"))
          {

              idAutomatic = true;
              indexAuto = i;

          }

     }

    return idAutomatic;

}

QVector<QStringList> Table::GetExec(const QString consultation, const int size, const QString msjGood, const QString msjBad) const
{

    QVector<QStringList> elements;
    QSqlQuery get;
    get.prepare(consultation);

     if(get.exec())
     {

         qDebug().noquote() <<QString("[+] %1").arg(msjGood);

          while(get.next())
          {

              QStringList list;

               for(int i = 0; i<= size; i++ )
               {

                   QString str = get.value(i).toByteArray();
                   list << str;

               }

              elements.push_back(list);

          }

     }else
     {

         qDebug().noquote() <<QString("[-] %1").arg(msjBad);

     }

    return elements;

}

QString Table::InitialPhraseOrder(const QString row, const QString order) const
{

    return QString("SELECT * FROM %1 ORDER BY %2 %3").arg(TableName, row, order);

}

QString Table::InitialPhraseElement(const QString rowName, const QString find) const
{

    return QString("SELECT * FROM %1 WHERE %2='%3'").arg(TableName, rowName, find);

}

QString Table::InitialPhraseElementOrder(const QString rowName, const QString find, const QString rowOrderName, const QString Order) const
{

    return QString("SELECT * FROM %1 WHERE %2='%3' ORDER BY %4 %5").arg(TableName, rowName, find, rowOrderName, Order);

}
