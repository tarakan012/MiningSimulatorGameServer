#ifndef USERDBDATASTRUCT_H
#define USERDBDATASTRUCT_H

#include "all_const.h"

#ifdef __cplusplus
extern "C" {
#endif

enum eUserDBData_Type {
    eUserDBData_None = 0,
    eUserDBData_Gold,
    eUserDBData_Energy,
    eUserDBData_End
};
struct SPODUserDBData {
    int n32DBId;
    int n32Gold;
    int n32Energy;
    bool bIfChangeArr[eUserDBData_End];
};
struct SUserDBData {
    SPODUserDBData sPODUserDBData;
    std::map<int, SComputerInfo> CompInfoMap;

    SUserDBData() {
        Clear();
    }

    void Clear() {
        memset(&sPODUserDBData, 0, sizeof(sPODUserDBData));
        CompInfoMap.clear();
    }

    bool EnergyIsFool() {
        return (sPODUserDBData.n32Energy >= 10) ? true : false;
    }

    void ChengeUserDBData(eUserDBData_Type type, int value) {
        switch (type) {
            case eUserDBData_Gold : {
                sPODUserDBData.n32Gold += value;
                break;
            }
            case eUserDBData_Energy : {
                sPODUserDBData.n32Energy += value;
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

