#ifndef ITEMRECORDMANAGER_H
#define ITEMRECORDMANAGER_H

#include "all_const.h"
#include "UserDBDataStruct.h"
#include "Logging.h"

#include <map>
#include <vector>

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif

        class CConfigManager {
        public:
            bool LoadShopCfg();

            bool LoadItemCfg();

            bool LoadStartSetCfg();

            bool Initialize();

            SUserDBData &GetStartSet() {
                return m_sStartSetCfg;
            }

            const std::map<int/*page_type*/, std::vector<SItemRecord>> &GetShopCfg() {
                return m_ShopCfgMap;
            }

            static CConfigManager &GetInstance();

        private:
            std::map<int, SItemRecord> m_ItemRecordMap;
            std::map<int/*page_type*/, std::vector<SItemRecord>> m_ShopCfgMap;
            SUserDBData m_sStartSetCfg;

        };


#ifdef __cplusplus
        }
#endif

    }
}

#endif /* ITEMRECORDMANAGER_H */

