//
// Created by cucaracha on 22.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_USERNETINFO_H
#define MININGSIMULATORGAMESERVER_USERNETINFO_H

#include "ServerMS/PreDefine.h"

struct SUserNetInfo {
    INT32 n32GCConnID;
    TIME_MILSEC m_tLastConnMilsec;

    SUserNetInfo(INT32 n32GCConnID)
            : n32GCConnID(n32GCConnID) {
    }

    SUserNetInfo() {
        Clear();
    }

    void Clear() {
        n32GCConnID = 0;
        m_tLastConnMilsec = 0;
    }

    bool IsValid() {
        return n32GCConnID != 0;
    }

    void operator=(const SUserNetInfo &sUserNetInfo) {
        n32GCConnID = sUserNetInfo.n32GCConnID;
    }

    bool operator<(const SUserNetInfo &sUserNetInfo) const {
        return n32GCConnID < sUserNetInfo.n32GCConnID;
    }
};

#endif //MININGSIMULATORGAMESERVER_USERNETINFO_H