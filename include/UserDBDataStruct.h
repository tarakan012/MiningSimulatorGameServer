#ifndef USERDBDATASTRUCT_H
#define USERDBDATASTRUCT_H

#include "PreDefine.h"
#include "all_const.h"
#include "ConfigConst.h"

#ifdef __cplusplus
extern "C" {
#endif

enum eUserDBData_Type {
    eUserDBData_None = 0,
    eUserDBData_Gold,
    eUserDBData_Energy,
    eUserDBData_LastTimeM,
    eUserDBData_End
};
struct SPODUserDBData {
    INT32 n32DBId;
    INT32 n32Gold;
    INT32 n32Energy;
    TIME_MILSEC tLastStartMining;
    bool bIfChangeArr[eUserDBData_End];
};
struct SUserDBData {
    SPODUserDBData sPODUserDBData;
    std::map<INT32, SComputerInfo> CompInfoMap;

    SUserDBData() {
        Clear();
    }

    void Clear() {
        memset(&sPODUserDBData, 0, sizeof(sPODUserDBData));
        CompInfoMap.clear();
    }

    bool EnergyIsFool() {
        return (sPODUserDBData.n32Energy >= nMAX_ENERGY) ? true : false;
    }

    void ChengeUserDBData(eUserDBData_Type type, INT32 value) {
        switch (type) {
            case eUserDBData_Gold : {
                sPODUserDBData.n32Gold += value;
                break;
            }
            case eUserDBData_Energy : {
                sPODUserDBData.n32Energy += value;
                break;
            }
            case eUserDBData_LastTimeM : {
                sPODUserDBData.tLastStartMining = value;
                break;
            }
        }
        sPODUserDBData.bIfChangeArr[type] = true;
    }

    SUserDBData &operator=(const SUserDBData &sUsrDBData) {
        if (this == &sUsrDBData) {
            return *this;
        }
        memcpy(&this->sPODUserDBData, &sUsrDBData.sPODUserDBData, sizeof(sUsrDBData.sPODUserDBData));
        CompInfoMap = sUsrDBData.CompInfoMap;
        return *this;
    }
};

#ifdef __cplusplus
}
#endif

#endif /* USERDBDATASTRUCT_H */

