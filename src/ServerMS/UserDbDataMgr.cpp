//
// Created by cucaracha on 21.04.18.
//

#include "ServerMS/UserDbDataMgr.h"

void CUserDbDataMgr::UpdateUserDbData(SUserDBData &userdb, std::stringstream &stream) {
    stream.clear();
    stream.str("");

    if (userdb.sPODUserDBData.bIfChangeArr[eUserDBData_Gold]) {
        CheckStreamAndInit(stream);
        stream << "btc=" << userdb.sPODUserDBData.n32Gold;
        userdb.sPODUserDBData.bIfChangeArr[eUserDBData_Gold] = true;
    }
    if (userdb.sPODUserDBData.bIfChangeArr[eUserDBData_Energy]) {
        CheckStreamAndInit(stream);
        stream << "energy=" << userdb.sPODUserDBData.n32Energy;
        userdb.sPODUserDBData.bIfChangeArr[eUserDBData_Energy] = true;
    }
    if (userdb.sPODUserDBData.bIfChangeArr[eUserDBData_LastTimeM]) {
        CheckStreamAndInit(stream);
        stream << "last_start_mining=to_timestamp(" << userdb.sPODUserDBData.tLastStartMining << ")";
        userdb.sPODUserDBData.bIfChangeArr[eUserDBData_LastTimeM] = true;
    }
    if (!stream.str().empty()) {
        stream << " where id=" << userdb.sPODUserDBData.n32DBId;
    }
}

void CUserDbDataMgr::CheckStreamAndInit(std::stringstream &mystream) {
    if (!mystream.str().empty()) {
        mystream << ",";
    } else {
        mystream << "update game_user set ";
    }
}