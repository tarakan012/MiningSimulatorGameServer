//
// Created by cucaracha on 21.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_USERDBDATAMGR_H
#define MININGSIMULATORGAMESERVER_USERDBDATAMGR_H

#include "ServerMS/UserDBDataStruct.h"
#include <sstream>

class CUserDbDataMgr {
public:
    static void UpdateUserDbData(SUserDBData &userdb, std::stringstream &stream);

    static void CheckStreamAndInit(std::stringstream &mystream);
};

#endif //MININGSIMULATORGAMESERVER_USERDBDATAMGR_H