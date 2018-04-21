#ifndef ITEMRECORDMANAGER_H
#define ITEMRECORDMANAGER_H

#include "all_const.h"
#include "UserDBDataStruct.h"
#include "util.h"

#include <map>

namespace MS {
    namespace Server {

#ifdef __cplusplus
extern "C" {
#endif

class CConfigManager
{
public:
    bool LoadShopCfg();
    bool LoadItemCfg();
    bool LoadStartSetCfg();
    bool Initialize();
    SUserDBData & GetStartSet()
    {
        return m_sStartSetCfg;
    }
    static CConfigManager & GetInstance();
private:
    std::map<int, SItemRecord> m_ItemRecordMap;
    std::map<int, SShopCfg> m_ShopCfgMap;
    SUserDBData m_sStartSetCfg;

};




#ifdef __cplusplus
}
#endif

    }
}

#endif /* ITEMRECORDMANAGER_H */

