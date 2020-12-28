#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>

#include <QTimer>
#include <QDir>
#include <QProcess>
#include <QDataStream>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>

#include "recvmessage.h"
#include "writelog.h"

class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QString conFile = 0, QObject *parent = 0);

private:
    _CONFIGURE cfg;
    WriteLog *log;
    RecvMessage *recvkissmessage;
    RecvMessage *recvmpssmessage;
    RecvEEWMessage *recveew;
    RecvUpdateMessage *recvupdatemessage;

    QVector<_STATION> staListVT_G;
    QVector<QString> netStaVTForGetIndex_G;
    QMultiMap<int, _QSCD_FOR_MULTIMAP> dataHouse_G;

    QTimer *systemTimer;

    void readCFG();
    void openDB();
    void readStationListfromDB();
    void readCriteriafromDB();
    void resetEvent();
    void makeEvent(_STATION);
    void checkEvent(QDateTime);
    int getLastEvid();
    void runSQL(QString, QString);
    void eventProcess(_EVENT_USING_GAL);

    QVector<_EVENT_USING_GAL> eventV;
    _EVENT event;
    bool isEvent;

    // About Database & table
    QSqlDatabase qscdDB;
    QSqlQueryModel *eventModel;
    QSqlQueryModel *networkModel;
    QSqlQueryModel *affiliationModel;
    QSqlQueryModel *siteModel;
    QSqlQueryModel *criteriaModel;

private slots:
    void rvEEWInfo(_EEWInfo);
    void doRepeatWork();
    void extractQSCD(QMultiMap<int, _QSCD_FOR_MULTIMAP>);
    void recvUpdateMessage(_UPDATE_MESSAGE);
};

#endif // MAINCLASS_H
