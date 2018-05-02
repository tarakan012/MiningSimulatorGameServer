#ifndef ITEMRECORDMANAGER_H
#define ITEMRECORDMANAGER_H

#include "ServerMS/all_const.h"
#include "ServerMS/UserDBDataStruct.h"
#include "ServerMS/Log/Logging.h"

namespace ServerMS {

    class CConfigManager {
    public:
        bool LoadShopCfg();

        bool LoadItemCfg();

        bool LoadStartSetCfg();

        bool Initialize();

        SUserDBData &GetStartSet() {
            return m_sStartSetCfg;
        }

        std::map<int/*page_type*/, std::map<INT32/*item_id*/, SItemRecord>> &GetShopCfg() {
            return m_ShopCfgMap;
        }

    private:
        std::map<int, SItemRecord> m_ItemRecordMap;
        std::map<int/*page_type*/, std::map<INT32/*item_id*/, SItemRecord>> m_ShopCfgMap;
        SUserDBData m_sStartSetCfg;

    };

}

#endif /* ITEMRECORDMANAGER_H */