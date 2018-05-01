#ifndef ALL_CONST
#define ALL_CONST

#include <cstring>

const int cMaxLenNameItem = 32;
const int cMaxItemInComp = 32;
const int cMaxItemInShop = 32;
const int cMaxCompInStartSet = 32;

enum eDBOperation {
    eOperationTypeNone = 0,
    eOperationTypeAdd,
    eOperationTypeUpdate,
    eOperationTypeDel
};

struct SItemRecord {

    int n32Id;
    int n32DBId;
    char szName[cMaxLenNameItem];
    int n32Lvl;
    int n32Type;
    int n32Icon;
    int n32MiningGold;
    int n32MiningResourses;
    int n32PowerConsumption;
    int n32ExpansionSlot;
    int n32LvlGrind;
    int n32Cost;
    int n32Strength;

    SItemRecord() {
        std::memset(this, 0, sizeof(*this));
    }
};

struct SPODComputerInfo {
    int n32ID;
    int n32MiningGold;
};

struct SComputerInfo {
    SPODComputerInfo sPODComputerInfo;
    int n32ID;
    int n32DBId;
    int n32MiningGold;
    std::map<int, SItemRecord> ItemRecordMap;

    SComputerInfo() {
        //memset(&sPODComputerInfo, 0, sizeof(sPODComputerInfo));
        Clear();
    }

    SComputerInfo &operator=(const SComputerInfo &sComputerInfo) {
        if (this == &sComputerInfo) {
            return *this;
        }
        memcpy(&this->sPODComputerInfo, &sComputerInfo.sPODComputerInfo, sizeof(sComputerInfo.sPODComputerInfo));
        ItemRecordMap = sComputerInfo.ItemRecordMap;
        return *this;
    }

    void Clear() {
        ItemRecordMap.clear();
        n32ID = 0;
        n32DBId = 0;
        n32MiningGold = 0;
    }
};

typedef std::map<int, SComputerInfo> ComputerInfoMap;

struct SStartSetCfg {
    ComputerInfoMap CompInfoMap;
};

#endif // ALL_CONST