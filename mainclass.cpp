#include "mainclass.h"

MainClass::MainClass(QString conFile, QObject *parent) : QObject(parent)
{
    activemq::library::ActiveMQCPP::initializeLibrary();

    cfg.configFileName = conFile;
    qRegisterMetaType< QMultiMap<int,_QSCD_FOR_MULTIMAP> >("QMultiMap<int,_QSCD_FOR_MULTIMAP>");
    qRegisterMetaType<_EEWInfo>("_EEWInfo");
    qRegisterMetaType<_UPDATE_MESSAGE>("_UPDATE_MESSAGE");

    log = new WriteLog();

    readCFG();

    qscdDB = QSqlDatabase::addDatabase("QMYSQL");
    qscdDB.setHostName(cfg.db_ip);
    qscdDB.setDatabaseName(cfg.db_name);
    qscdDB.setUserName(cfg.db_user);
    qscdDB.setPassword(cfg.db_passwd);

    eventModel = new QSqlQueryModel();
    networkModel = new QSqlQueryModel();
    affiliationModel = new QSqlQueryModel();
    siteModel = new QSqlQueryModel();
    criteriaModel = new QSqlQueryModel();

    readStationListfromDB();
    readCriteriafromDB();
    resetEvent();

    dataHouse_G.clear();
    systemTimer = new QTimer(this);
    systemTimer->start(1000);
    connect(systemTimer, SIGNAL(timeout()), this, SLOT(doRepeatWork()));

    // consumer
    if(cfg.kiss_amq_topic != "")
    {
        QString kissFailover = "failover:(tcp://" + cfg.kiss_amq_ip + ":" + cfg.kiss_amq_port + ")";

        recvkissmessage = new RecvMessage;
        if(!recvkissmessage->isRunning())
        {
            recvkissmessage->setup(kissFailover, cfg.kiss_amq_user, cfg.kiss_amq_passwd, cfg.kiss_amq_topic, true, false);
            connect(recvkissmessage, SIGNAL(sendQSCDtoMain(QMultiMap<int, _QSCD_FOR_MULTIMAP>)), this, SLOT(extractQSCD(QMultiMap<int, _QSCD_FOR_MULTIMAP>)));
            recvkissmessage->start();
        }
    }

    if(cfg.mpss_amq_topic != "")
    {
        QString mpssFailover = "failover:(tcp://" + cfg.mpss_amq_ip + ":" + cfg.mpss_amq_port + ")";

        recvmpssmessage = new RecvMessage;
        if(!recvmpssmessage->isRunning())
        {
            recvmpssmessage->setup(mpssFailover, cfg.mpss_amq_user, cfg.mpss_amq_passwd, cfg.mpss_amq_topic, true, false);
            connect(recvmpssmessage, SIGNAL(sendQSCDtoMain(QMultiMap<int, _QSCD_FOR_MULTIMAP>)), this, SLOT(extractQSCD(QMultiMap<int, _QSCD_FOR_MULTIMAP>)));
            recvmpssmessage->start();
        }
    }

    if(cfg.eew_amq_topic != "")
    {
        QString eewFailover = "failover:(tcp://" + cfg.eew_amq_ip + ":" + cfg.eew_amq_port + ")";

        recveew = new RecvEEWMessage;
        if(!recveew->isRunning())
        {
            recveew->setup(eewFailover, cfg.eew_amq_user, cfg.eew_amq_passwd, cfg.eew_amq_topic, true, false);
            connect(recveew, SIGNAL(_rvEEWInfo(_EEWInfo)), this, SLOT(rvEEWInfo(_EEWInfo)));
            recveew->start();
        }
    }

/*
    if(cfg.update_amq_topic != "")
    {
        QString updateFailover = "failover:(tcp://" + cfg.update_amq_ip + ":" + cfg.update_amq_port + ")";

        recvupdatemessage = new RecvUpdateMessage;
        if(!recvupdatemessage->isRunning())
        {
            recvupdatemessage->setup(updateFailover, cfg.update_amq_user, cfg.update_amq_passwd, cfg.update_amq_topic, true, false);
            connect(recvupdatemessage, SIGNAL(_rvUpdateInfo(_UPDATE_MESSAGE)), this, SLOT(recvUpdateMessage(_UPDATE_MESSAGE)));
            recvupdatemessage->start();
        }

    }
*/

    log->write(cfg.logDir, "======================================================");
    log->write(cfg.logDir, "RTICOM2CORE Started.");
}

void MainClass::recvUpdateMessage(_UPDATE_MESSAGE message)
{
    if(QString(message.MType).startsWith("S") && message.value == 1)
    {
        log->write(cfg.logDir, "Recevied a update message for stations list.");
        log->write(cfg.logDir, "Restart RTICOM2 after reloading a stations list.");
        readStationListfromDB();
        resetEvent();
    }
    else if(QString(message.MType).startsWith("C") && message.value == 1)
    {
        log->write(cfg.logDir, "Recevied a update message for event criteria.");
        readCriteriafromDB();
        resetEvent();
    }
}

void MainClass::readCFG()
{
    QFile file(cfg.configFileName);
    if(!file.exists())
    {
        qDebug() << "Failed configuration. Parameter file doesn't exists.";
        exit(1);
    }
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line, _line;

        while(!stream.atEnd())
        {
            line = stream.readLine();
            _line = line.simplified();

            if(_line.startsWith(" ") || _line.startsWith("#"))
                continue;
            else if(_line.startsWith("HOME_DIR"))
            {
                cfg.homeDir = _line.section("=", 1, 1);
                cfg.logDir = cfg.homeDir + "/logs";
            }
            else if(_line.startsWith("EVENT_DIR"))
                cfg.eventDir = _line.section("=", 1, 1);
            else if(_line.startsWith("DIFF_TIME"))
                cfg.timeDiffForMon = _line.section("=", 1, 1).toInt();

            else if(_line.startsWith("DB_IP"))
                cfg.db_ip = _line.section("=", 1, 1);
            else if(_line.startsWith("DB_NAME"))
                cfg.db_name = _line.section("=", 1, 1);
            else if(_line.startsWith("DB_USERNAME"))
                cfg.db_user = _line.section("=", 1, 1);
            else if(_line.startsWith("DB_PASSWD"))
                cfg.db_passwd = _line.section("=", 1, 1);

            else if(_line.startsWith("UPDATE_AMQ_IP"))
                cfg.update_amq_ip = _line.section("=", 1, 1);
            else if(_line.startsWith("UPDATE_AMQ_PORT"))
                cfg.update_amq_port = _line.section("=", 1, 1);
            else if(_line.startsWith("UPDATE_AMQ_USERNAME"))
                cfg.update_amq_user = _line.section("=", 1, 1);
            else if(_line.startsWith("UPDATE_AMQ_PASSWD"))
                cfg.update_amq_passwd = _line.section("=", 1, 1);
            else if(_line.startsWith("UPDATE_AMQ_TOPIC"))
                cfg.update_amq_topic = _line.section("=", 1, 1);

            else if(_line.startsWith("KISS_AMQ_IP"))
                cfg.kiss_amq_ip = _line.section("=", 1, 1);
            else if(_line.startsWith("KISS_AMQ_PORT"))
                cfg.kiss_amq_port = _line.section("=", 1, 1);
            else if(_line.startsWith("KISS_AMQ_USERNAME"))
                cfg.kiss_amq_user = _line.section("=", 1, 1);
            else if(_line.startsWith("KISS_AMQ_PASSWD"))
                cfg.kiss_amq_passwd = _line.section("=", 1, 1);
            else if(_line.startsWith("KISS_AMQ_TOPIC"))
                cfg.kiss_amq_topic = _line.section("=", 1, 1);

            else if(_line.startsWith("MPSS_AMQ_IP"))
                cfg.mpss_amq_ip = _line.section("=", 1, 1);
            else if(_line.startsWith("MPSS_AMQ_PORT"))
                cfg.mpss_amq_port = _line.section("=", 1, 1);
            else if(_line.startsWith("MPSS_AMQ_USERNAME"))
                cfg.mpss_amq_user = _line.section("=", 1, 1);
            else if(_line.startsWith("MPSS_AMQ_PASSWD"))
                cfg.mpss_amq_passwd = _line.section("=", 1, 1);
            else if(_line.startsWith("MPSS_AMQ_TOPIC"))
                cfg.mpss_amq_topic = _line.section("=", 1, 1);

            else if(_line.startsWith("EEW_AMQ_IP"))
                cfg.eew_amq_ip = _line.section("=", 1, 1);
            else if(_line.startsWith("EEW_AMQ_PORT"))
                cfg.eew_amq_port = _line.section("=", 1, 1);
            else if(_line.startsWith("EEW_AMQ_USERNAME"))
                cfg.eew_amq_user = _line.section("=", 1, 1);
            else if(_line.startsWith("EEW_AMQ_PASSWD"))
                cfg.eew_amq_passwd = _line.section("=", 1, 1);
            else if(_line.startsWith("EEW_AMQ_TOPIC"))
                cfg.eew_amq_topic = _line.section("=", 1, 1);

            else if(_line.startsWith("CHANNEL"))
            {
                if(_line.section("=", 1, 1).left(1).startsWith("Z")) cfg.monChanID = 0;
                else if(_line.section("=", 1, 1).startsWith("N")) cfg.monChanID = 1;
                else if(_line.section("=", 1, 1).startsWith("E")) cfg.monChanID = 2;
                else if(_line.section("=", 1, 1).startsWith("H")) cfg.monChanID = 3;
                else if(_line.section("=", 1, 1).startsWith("T")) cfg.monChanID = 4;
            }
        }
        file.close();
    }
}

void MainClass::openDB()
{
    if(!qscdDB.open())
    {
        log->write(cfg.logDir, "Error connecting to DB: " + qscdDB.lastError().text());
    }
}

void MainClass::readStationListfromDB()
{
    staListVT_G.clear();
    netStaVTForGetIndex_G.clear();

    QString query;
    query = "SELECT * FROM NETWORK";
    openDB();
    networkModel->setQuery(query);

    int count = 0;

    if(networkModel->rowCount() > 0)
    {
        for(int i=0;i<networkModel->rowCount();i++)
        {
            QString network = networkModel->record(i).value("net").toString();
            query = "SELECT * FROM AFFILIATION where net='" + network + "'";
            affiliationModel->setQuery(query);

            for(int j=0;j<affiliationModel->rowCount();j++)
            {
                QString affiliation = affiliationModel->record(j).value("aff").toString();
                QString affiliationName = affiliationModel->record(j).value("affname").toString();
                double lat = affiliationModel->record(j).value("lat").toDouble();
                double lon = affiliationModel->record(j).value("lon").toDouble();
                double elev = affiliationModel->record(j).value("elev").toDouble();
                query = "SELECT * FROM SITE where aff='" + affiliation + "'";
                siteModel->setQuery(query);

                for(int k=0;k<siteModel->rowCount();k++)
                {
		    if(siteModel->record(k).value("statype").toString().startsWith("G"))
                    {
			_STATION sta;
			sta.index = count;
			sta.sta = siteModel->record(k).value("sta").toString();
			netStaVTForGetIndex_G.push_back(sta.sta);
			sta.comment = affiliationName;
			sta.lat = lat;
			sta.lon = lon;
			sta.elev = elev;
			sta.staType = siteModel->record(k).value("statype").toString();
			sta.inUse = siteModel->record(k).value("inuse").toInt();
			sta.lastPGA = 0;
			sta.lastPGATime = 0;

			staListVT_G.push_back(sta);
			count++;
		    }
                }
            }
        }
    }

    log->write(cfg.logDir, "Read Station List (Ground) from DB. Number of Station : " + QString::number(staListVT_G.count()));
}

void MainClass::readCriteriafromDB()
{
    QString query;
    query = "SELECT * FROM CRITERIA";
    openDB();
    criteriaModel->setQuery(query);

    cfg.inSeconds = criteriaModel->record(0).value("inseconds").toInt();
    cfg.numSta = criteriaModel->record(0).value("numsta").toInt();
    cfg.thresholdG = criteriaModel->record(0).value("thresholdG").toDouble();
    cfg.distance = criteriaModel->record(0).value("distance").toInt();
    cfg.thresholdM = criteriaModel->record(0).value("thresholdM").toDouble();

    log->write(cfg.logDir, "Read Criteria List from DB.");
    log->write(cfg.logDir, "In seconds : " + QString::number(cfg.inSeconds));
    log->write(cfg.logDir, "Number of stations : " + QString::number(cfg.numSta));
    log->write(cfg.logDir, "Threshold(gal) : " + QString::number(cfg.thresholdG, 'f', 1));
    log->write(cfg.logDir, "Distance for each stations : " + QString::number(cfg.distance));
    log->write(cfg.logDir, "Threshold(eew magnitude) : " + QString::number(cfg.thresholdM, 'f', 1));
}

void MainClass::resetEvent()
{
    isEvent = false;
    eventV.clear();
    event.evid = 0;
}

void MainClass::extractQSCD(QMultiMap<int, _QSCD_FOR_MULTIMAP> mmFromAMQ)
{
    QMapIterator<int, _QSCD_FOR_MULTIMAP> i(mmFromAMQ);
    while(i.hasNext())
    {
        i.next();
        dataHouse_G.insert(i.key(), i.value());
    }
}

void MainClass::doRepeatWork()
{
    QDateTime systemTime = QDateTime::currentDateTimeUtc();
    QDateTime dataTime = systemTime.addSecs(- cfg.timeDiffForMon); // GMT

    QList<_QSCD_FOR_MULTIMAP> pgaList_G = dataHouse_G.values(dataTime.toTime_t());

    int realstanum = 0;

    if(pgaList_G.size() != 0)
    {
        int staIndex, repeatIndex;
        QVector<int> indexVector;

        if(isEvent && dataTime.toTime_t() >= event.epochtime + EVENT_SECONDS_FOR_START)
        {
            QFile evtdataGFile(event.datafilepath +  "/SRC/PGA_G.dat");
            evtdataGFile.open(QIODevice::WriteOnly | QIODevice::Append);
            QTextStream evtdataGout(&evtdataGFile);

	    for(int i=0;i<pgaList_G.size();i++)
	    {
		evtdataGout << QString::number(dataTime.toTime_t()) << " " << pgaList_G.at(i).sta << "=" << QString::number(pgaList_G.at(i).pga[0], 'f', 6) << ":"
                        << QString::number(pgaList_G.at(i).pga[1], 'f', 6) << ":"
                        << QString::number(pgaList_G.at(i).pga[2], 'f', 6) << ":"
                        << QString::number(pgaList_G.at(i).pga[3], 'f', 6) << ":"
                        << QString::number(pgaList_G.at(i).pga[4], 'f', 6)
                        << "\n";
	    }
            evtdataGFile.close();

	    QFile evtdataBFile(event.datafilepath + "/SRC/PGA_G_B.dat");
	    evtdataBFile.open(QIODevice::WriteOnly | QIODevice::Append);
	    QDataStream stream(&evtdataBFile);
	    _QSCD_FOR_BIN data;
	    data.time = dataTime.toTime_t();
	    for(int i=0;i<pgaList_G.size();i++)
	    {
		qsnprintf(data.sta, sizeof(data.sta), "%s", pgaList_G.at(i).sta.toUtf8().constData());
		data.pga[0] = pgaList_G.at(i).pga[0];
		data.pga[1] = pgaList_G.at(i).pga[1];
		data.pga[2] = pgaList_G.at(i).pga[2];
		data.pga[3] = pgaList_G.at(i).pga[3];
		data.pga[4] = pgaList_G.at(i).pga[4];
		stream.writeRawData((char*)&data, sizeof(_QSCD_FOR_BIN));
	    }
	    evtdataBFile.close();

	    int timeDiff = dataTime.toTime_t() - event.epochtime;

	    // end event
	    if(timeDiff > EVENT_DURATION)
	    {
		log->write(cfg.logDir, "==================== END OF THIS EVENT =====================");
		QString command;
		command = "/opt/RTICOM2CORE/bin/RTICOM2ANALYST " + event.datafilepath;
		system(command.toLatin1().data());

		/*
		command = "/opt/RTICOM2CORE/bin/getQSCDtoBinFile.sh " + QString::number(event.epochtime + EVENT_SECONDS_FOR_START) + " " + QString::number(EVENT_DURATION);
		system(command.toLatin1().data());
		QDateTime st;
		st.setTime_t(event.epochtime + EVENT_SECONDS_FOR_START);
		command = "mv /opt/RTICOM2CORE/data/" + st.toString("yyyyMMddhhmmss") + "_B.dat " + cfg.eventDir + event.eventtime.toString("/yyyy/MM/") + QString::number(event.evid) + "/SRC/PGA_G_bin.dat";
		system(command.toLatin1().data());
		*/

		command = "tar cvzf " + cfg.eventDir + event.eventtime.toString("/yyyy/MM/") + QString::number(event.evid) + ".tar.gz " + cfg.eventDir + event.eventtime.toString("/yyyy/MM/") + QString::number(event.evid);
		system(command.toLatin1().data());

		resetEvent();
	     }
	}

        for(int i=0;i<pgaList_G.size();i++)
        {
            staIndex = netStaVTForGetIndex_G.indexOf(pgaList_G.at(i).sta);
            repeatIndex = indexVector.indexOf(staIndex);

            if(staIndex != -1 && staListVT_G.at(staIndex).inUse == 1 && repeatIndex == -1)
            {
                realstanum++;
                indexVector.push_back(staIndex);

                _STATION _sta;
                _sta = staListVT_G.at(staIndex);
                _sta.lastPGATime = dataTime.toTime_t();
                _sta.lastPGA = pgaList_G.at(i).pga[cfg.monChanID];

                makeEvent(_sta);
                staListVT_G.replace(staIndex, _sta);
            }
        }

        if(!isEvent) checkEvent(dataTime);
    }

    log->write(cfg.logDir, "TIME=" + QString::number(dataTime.toTime_t()) + "  NUM=" + QString::number(realstanum) + " of " + QString::number(pgaList_G.size()));

    // remove mmap
    if(!dataHouse_G.isEmpty())
    {
        QMultiMap<int, _QSCD_FOR_MULTIMAP>::iterator iter;
        QMultiMap<int, _QSCD_FOR_MULTIMAP>::iterator untilIter;
        untilIter = dataHouse_G.lowerBound(dataTime.toTime_t() - 20);

        for(iter = dataHouse_G.begin() ; untilIter != iter;)
        {
            QMultiMap<int, _QSCD_FOR_MULTIMAP>::iterator thisIter;
            thisIter = iter;
            iter++;
            dataHouse_G.erase(thisIter);
        }
    }
}

void MainClass::makeEvent(_STATION tempSta)
{
    // make a EVENT VECTOR (eventV)
    if(tempSta.lastPGA >= cfg.thresholdG && !isEvent)
    {
        if(eventV.isEmpty())
        {
            _EVENT_USING_GAL ev;
            ev.numOverDist = 0;
            ev.triggerTime = tempSta.lastPGATime;
            ev.originTime = 0;
            ev.staIndexV.push_back(tempSta.index);
            ev.timeV.push_back(tempSta.lastPGATime);
            ev.pgaV.push_back(tempSta.lastPGA);
            eventV.push_back(ev);
        }
        else
        {
            for(int k=0;k<eventV.count();k++)
            {
                int timeDiff = tempSta.lastPGATime - eventV.at(k).triggerTime;

                if(timeDiff <= cfg.inSeconds)
                {
                    _EVENT_USING_GAL ev = eventV.at(k);

                    int isExist = ev.staIndexV.indexOf(tempSta.index);

                    if(isExist == -1)
                    {
                        ev.staIndexV.push_back(tempSta.index);
                        ev.timeV.push_back(tempSta.lastPGATime);
                        ev.pgaV.push_back(tempSta.lastPGA);

                        // get number over distance
                        for(int xx = 0; xx < ev.staIndexV.count() - 1 ; xx++)
                        {
                            for(int yy = xx + 1 ; yy < ev.staIndexV.count() ; yy++)
                            {
                                double dist = getDistance(staListVT_G.at(ev.staIndexV.at(xx)).lat,
                                                          staListVT_G.at(ev.staIndexV.at(xx)).lon,
                                                          staListVT_G.at(ev.staIndexV.at(yy)).lat,
                                                          staListVT_G.at(ev.staIndexV.at(yy)).lon);

                                if(dist >= cfg.distance)
                                    ev.numOverDist++;
                            }
                        }

                        eventV.replace(k, ev);
                    }
                }
            }

            _EVENT_USING_GAL ev;
            ev.numOverDist = 0;
            ev.triggerTime = tempSta.lastPGATime;
            ev.originTime = 0;
            ev.staIndexV.push_back(tempSta.index);
            ev.timeV.push_back(tempSta.lastPGATime);
            ev.pgaV.push_back(tempSta.lastPGA);
            eventV.push_back(ev);
        }
    }
}

void MainClass::checkEvent(QDateTime dataTime)
{
    // check event
    if(!eventV.isEmpty() && !isEvent)
    {
        for(int k=0;k<eventV.count();k++)
        {
            int timeDiff = dataTime.toTime_t() - eventV.at(k).triggerTime;
            if(timeDiff <= cfg.inSeconds && eventV.at(k).staIndexV.count() >= cfg.numSta &&
                    (eventV.at(k).staIndexV.count() - eventV.at(k).numOverDist) >= cfg.numSta )
            {
                // check distance
                _EVENT_USING_GAL ev = eventV.at(k);
                ev.originTime = dataTime.toTime_t(); // epoch (GMT)
                eventV.replace(k, ev);
                isEvent = true;

                event.evid = getLastEvid() + 1;
                event.epochtime = ev.originTime;
                event.eventtime.setTime_t(ev.originTime);
                event.eventtype = "G";
                event.insec = cfg.inSeconds;
                event.numsta = cfg.numSta;
                event.thresholdG = cfg.thresholdG;
                event.distance = cfg.distance;

                eventProcess(ev);

                eventV.clear();
                break;
            }
            else
            {
                eventV.remove(k);
            }
        }
    }
}

int MainClass::getLastEvid()
{
    QString query = "SELECT max(evid) FROM EVENT";
    openDB();
    eventModel->setQuery(query);
    return eventModel->record(0).value("max(evid)").toInt();
}

void MainClass::rvEEWInfo(_EEWInfo eewInfo)
{
    if(eewInfo.magnitude < cfg.thresholdM)
        return;

    if(!isEvent)
    {
        resetEvent();
        isEvent = true;
        event.evid = getLastEvid() + 1;
        event.epochtime = eewInfo.origin_time;
        event.eventtime.setTime_t(eewInfo.origin_time);
        event.eewevid = eewInfo.eew_evid;
        event.eventtype = "E";
        event.origintime = eewInfo.origin_time;
        event.lat = eewInfo.latitude;
        event.lon = eewInfo.longitude;
        event.mag = eewInfo.magnitude;
        event.nsta = eewInfo.number_stations;
        event.thresholdM = cfg.thresholdM;
        QProcess process;
        QString cmd = cfg.homeDir + "/bin/" + find_loc_program + " " + QString::number(event.lat, 'f', 4) + " " + QString::number(event.lon, 'f', 4);
        process.start(cmd);
        process.waitForFinished(-1); // will wait forever until finished
        QString stdout = process.readAllStandardOutput();
        int leng = stdout.length();
        event.eventname = stdout.left(leng-1);

        _EVENT_USING_GAL ev; // null
        eventProcess(ev);
    }
    else
    {
        if(eewInfo.message_type == UPDATE && event.eewevid == eewInfo.eew_evid)
        {
            event.origintime = eewInfo.origin_time;
            event.lat = eewInfo.latitude;
            event.lon = eewInfo.longitude;
            event.mag = eewInfo.magnitude;
            event.nsta = eewInfo.number_stations;
            QProcess process;
            QString cmd = cfg.homeDir + "/bin/" + find_loc_program + " " + QString::number(event.lat, 'f', 4) + " " + QString::number(event.lon, 'f', 4);
            process.start(cmd);
            process.waitForFinished(-1); // will wait forever until finished
            QString stdout = process.readAllStandardOutput();
            int leng = stdout.length();
            event.eventname = stdout.left(leng-1);

            // logging
            log->write(cfg.logDir, "==================== UPDATE THE EVENT BY EEW =====================");
            log->write(cfg.logDir, "EVID:" + QString::number(event.evid));
            log->write(cfg.logDir, "EVENT_TIME:" + event.eventtime.toString("yyyy/MM/dd hh:mm:ss"));
            log->write(cfg.logDir, "EVENT_TIME(EPOCHTIME):" + QString::number(event.epochtime));
            log->write(cfg.logDir, "EVENT_TYPE=E");
            log->write(cfg.logDir, "EEW_INFO:" + QString::number(event.eewevid) + "/" + QString::number(event.origintime) + "/" + QString::number(event.lat, 'f', 4)
                       + "/" + QString::number(event.lon, 'f', 4) + "/" + QString::number(event.mag, 'f', 2) + "/" + QString::number(event.nsta));
            log->write(cfg.logDir, "MAG_THRESHOLD:" + QString::number(cfg.thresholdM, 'f', 2));
            log->write(cfg.logDir, "DATA_FILE_PATH:" + event.datafilepath);
            log->write(cfg.logDir, "====================================================================");

            // header file
            QFile evtHeaderFile(event.datafilepath + "/SRC/header.dat");
            evtHeaderFile.open(QIODevice::WriteOnly | QIODevice::Append);
            QTextStream headerout(&evtHeaderFile);
            headerout << "EEW_INFO=" << QString::number(event.eewevid) << ":" << QString::number(event.origintime) << ":" << QString::number(event.lat, 'f', 4) << ":"
                << QString::number(event.lon, 'f', 4) << ":" << QString::number(event.mag, 'f', 2) << ":" << QString::number(event.nsta) << "\n";
            evtHeaderFile.close();

            // insert query
            QString query = "UPDATE EVENT SET "
                    "origintime = " + QString::number(event.origintime) +
                    ", lat = " + QString::number(event.lat, 'f', 4) +
                    ", lon = " + QString::number(event.lon, 'f', 4) +
                    ", mag = " + QString::number(event.mag, 'f', 2) +
                    ", nsta = " + QString::number(event.nsta) +
                    ", eventname = '" + event.eventname +
                    "' WHERE evid = " + QString::number(event.evid);
            runSQL(query, "event");
        }
    }
}

void MainClass::eventProcess(_EVENT_USING_GAL ev)
{
    // save event header to binary file
    QString evtFilePathS = cfg.eventDir + event.eventtime.toString("/yyyy/MM/") + QString::number(event.evid) + "/SRC";
    QString evtFilePathS2 = cfg.eventDir + event.eventtime.toString("/yyyy/MM/") + QString::number(event.evid);
    QDir evtFilePathD(evtFilePathS);
    if(!evtFilePathD.exists())
        evtFilePathD.mkpath(".");
    event.datafilepath = evtFilePathS2;

    // logging
    if(event.eventtype.startsWith("G"))
    {
        log->write(cfg.logDir, "==================== DETECTED NEW EVENT BY GAL =====================");
        log->write(cfg.logDir, "EVENT_TIME:" + event.eventtime.toString("yyyy/MM/dd hh:mm:ss"));
        log->write(cfg.logDir, "EVENT_TIME(EPOCHTIME):" + QString::number(event.epochtime));
        log->write(cfg.logDir, "EVENT_TYPE=G");
        log->write(cfg.logDir, "EVENT_CONDITION=" + QString::number(cfg.inSeconds) + ":" + QString::number(cfg.numSta) + ":"
                   + QString::number(cfg.thresholdG, 'f', 2) + ":" + QString::number(cfg.distance));
        for(int j=0;j<ev.staIndexV.count();j++)
        {
            QDateTime t; t.setTime_t(ev.timeV.at(j));
            log->write(cfg.logDir, "FIRST_EVENT_INFO=" + QString::number(ev.timeV.at(j)) + ":" + staListVT_G.at(ev.staIndexV.at(j)).sta
                       + ":" + staListVT_G.at(ev.staIndexV.at(j)).comment + ":" + QString::number(ev.pgaV.at(j), 'f', 6));
        }
        log->write(cfg.logDir, "DATA_FILE_PATH:" + event.datafilepath);
        log->write(cfg.logDir, "====================================================================");
    }
    else if(event.eventtype.startsWith("E"))
    {
        log->write(cfg.logDir, "==================== DETECTED NEW EVENT BY EEW =====================");
        log->write(cfg.logDir, "EVID:" + QString::number(event.evid));
        log->write(cfg.logDir, "EVENT_TIME:" + event.eventtime.toString("yyyy/MM/dd hh:mm:ss"));
        log->write(cfg.logDir, "EVENT_TIME(EPOCHTIME):" + QString::number(event.epochtime));
        log->write(cfg.logDir, "EVENT_TYPE=E");
        log->write(cfg.logDir, "EEW_INFO:" + QString::number(event.eewevid) + "/" + QString::number(event.origintime) + "/" + QString::number(event.lat, 'f', 4)
                   + "/" + QString::number(event.lon, 'f', 4) + "/" + QString::number(event.mag, 'f', 2) + "/" + QString::number(event.nsta));
        log->write(cfg.logDir, "MAG_THRESHOLD:" + QString::number(cfg.thresholdM, 'f', 2));
        log->write(cfg.logDir, "DATA_FILE_PATH:" + event.datafilepath);
        log->write(cfg.logDir, "====================================================================");
    }

    // header file
    QFile evtHeaderFile(event.datafilepath + "/SRC/header.dat");
    evtHeaderFile.open(QIODevice::WriteOnly);
    QTextStream headerout(&evtHeaderFile);
    headerout << "EVID=" << QString::number(event.evid) << "\n";
    headerout << "EVENT_TIME=" << QString::number(event.epochtime) << "\n";
    headerout << "DATA_START_TIME=" << QString::number(event.epochtime + EVENT_SECONDS_FOR_START) << "\n";
    headerout << "DURATION=" << QString::number(EVENT_DURATION) << "\n";
    if(event.eventtype.startsWith("G"))
    {
        headerout << "EVENT_TYPE=G" << "\n";
        headerout << "EVENT_CONDITION=" << QString::number(cfg.inSeconds) << ":" << QString::number(cfg.numSta) << ":"
            << QString::number(cfg.thresholdG, 'f', 2) << ":" << QString::number(cfg.distance) << "\n";

        for(int j=0;j<ev.staIndexV.count();j++)
        {
            QDateTime t; t.setTime_t(ev.timeV.at(j));
            headerout << "FIRST_EVENT_INFO=" << QString::number(ev.timeV.at(j)) << ":" << staListVT_G.at(ev.staIndexV.at(j)).sta
                << ":" << staListVT_G.at(ev.staIndexV.at(j)).comment << ":" << QString::number(ev.pgaV.at(j), 'f', 6) << "\n";
        }
    }
    else if(event.eventtype.startsWith("E"))
    {
        headerout << "EVENT_TYPE=E" << "\n";
        headerout << "MAG_THRESHOLD=" << QString::number(cfg.thresholdM, 'f', 2) << "\n";
        headerout << "EEW_INFO=" << QString::number(event.eewevid) << ":" << QString::number(event.origintime) << ":" << QString::number(event.lat, 'f', 4) << ":"
            << QString::number(event.lon, 'f', 4) << ":" << QString::number(event.mag, 'f', 2) << ":" << QString::number(event.nsta) << "\n";
    }
    evtHeaderFile.close();

    // stationfile for ground
    QFile evtStaGFile(event.datafilepath +  "/SRC/stalist_G.dat");
    evtStaGFile.open(QIODevice::WriteOnly);
    QTextStream staListGout(&evtStaGFile);
    for(int j=0;j<staListVT_G.count();j++)
    {
        staListGout << staListVT_G.at(j).sta << "="
             << staListVT_G.at(j).lat << "=" << staListVT_G.at(j).lon << "=" << staListVT_G.at(j).elev << "="
             << staListVT_G.at(j).comment << "=" << QString::number(staListVT_G.at(j).inUse) << "\n";
    }
    evtStaGFile.close();

    // data file for ground
    QFile evtdataGFile(event.datafilepath +  "/SRC/PGA_G.dat");
    evtdataGFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream evtdataGout(&evtdataGFile);
    for(int j=event.epochtime + EVENT_SECONDS_FOR_START;j<=event.epochtime+1;j++)
    {
        QList<_QSCD_FOR_MULTIMAP> values = dataHouse_G.values(j);

        for(int x=0;x<values.count();x++)
            evtdataGout << QString::number(j) << " " << values.at(x).sta << "=" << QString::number(values.at(x).pga[0], 'f', 6) << ":"
                    << QString::number(values.at(x).pga[1], 'f', 6) << ":"
                    << QString::number(values.at(x).pga[2], 'f', 6) << ":"
                    << QString::number(values.at(x).pga[3], 'f', 6) << ":"
                    << QString::number(values.at(x).pga[4], 'f', 6)
                    << "\n";
    }
    evtdataGFile.close();

    QFile evtdataBFile(event.datafilepath + "/SRC/PGA_G_B.dat");
    evtdataBFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QDataStream stream(&evtdataBFile);
    for(int j=event.epochtime + EVENT_SECONDS_FOR_START;j<=event.epochtime+1;j++)
    {
        QList<_QSCD_FOR_MULTIMAP> values = dataHouse_G.values(j);

        for(int x=0;x<values.count();x++)
	{
		_QSCD_FOR_BIN data;
		data.time = j;
		qsnprintf(data.sta, sizeof(data.sta), "%s", values.at(x).sta.toUtf8().constData());
		data.pga[0] = values.at(x).pga[0];
		data.pga[1] = values.at(x).pga[1];
		data.pga[2] = values.at(x).pga[2];
		data.pga[3] = values.at(x).pga[3];
		data.pga[4] = values.at(x).pga[4];
    		stream.writeRawData((char*)&data, sizeof(_QSCD_FOR_BIN));
	}
    }
    evtdataBFile.close();

    // insert query
    QString query;
    if(event.eventtype.startsWith("G"))
    {
        query = "INSERT INTO EVENT "
                            "(evid, eventtime, epochtime, eventtype, insec, "
                            "numsta, distance, thresholdG, datafilepath, lddate) values (" +
                    QString::number(event.evid) + ", '" + event.eventtime.toString("yyyy-MM-dd hh:mm:ss") + "', " +
                    QString::number(event.epochtime) + ", 'G', " + QString::number(event.insec) + ", " + QString::number(event.numsta) +
		    ", " + QString::number(event.distance) +
                    ", " + QString::number(event.thresholdG, 'f', 2) +
                    ", '" + event.datafilepath + "', '" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";
    }
    else if(event.eventtype.startsWith("E"))
    {
        query = "INSERT INTO EVENT "
                            "(evid, eventtime, epochtime, eventtype, eewevid, "
                            "origintime, lat, "
                            "lon, mag, nsta, eventname, "
                            "thresholdM, datafilepath, lddate) values (" +
                    QString::number(event.evid) + ", '" + event.eventtime.toString("yyyy-MM-dd hh:mm:ss") + "', " +
                    QString::number(event.epochtime) + ", 'E', " + QString::number(event.eewevid) + ", " + QString::number(event.origintime) +
                    ", " + QString::number(event.lat, 'f', 4) + ", " + QString::number(event.lon, 'f', 4) + ", " + QString::number(event.mag, 'f', 2) +
                    ", " + QString::number(event.nsta) + ", '" + event.eventname + "', " + QString::number(event.thresholdM, 'f', 2) +
                    ", '" + event.datafilepath + "', '" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";
    }
    runSQL(query, "event");
}

void MainClass::runSQL(QString query, QString tableName)
{
    openDB();
    if(tableName.startsWith("event"))
    {
        eventModel->setQuery(query);
        if(eventModel->lastError().isValid())
        {
            qDebug() << eventModel->lastError();
            log->write(cfg.logDir, eventModel->lastError().text());
            log->write(cfg.logDir, "SQL ERROR\n" + query);
            return;
        }
        log->write(cfg.logDir, "SQL COMPLETE\n" + query);
    }
}
