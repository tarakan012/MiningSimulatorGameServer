//
// Created by cucaracha on 26.04.18.
//

#include "ShopManager.h"
#include "ConfigManager.h"
#include "AllErrorCode.h"
#include "User.h"

namespace MS {
    namespace Server {

        IMPLEMENT_SINGLETON(CShopManager)

        INT32 CShopManager::Buy(CUser *user, INT32 item_id) {
            auto &cShopCfg = CConfigManager::GetInstance().GetShopCfg();
            for (auto &page_pair : cShopCfg) {
                std::map<INT32, SItemRecord> &PageShopMap = page_pair.second;
                auto iter = PageShopMap.find(item_id);
                if (iter != PageShopMap.end()) {
                    auto &item = PageShopMap[item_id];
                    if (user->GetGold() >= item.n32Cost) {
                        user->GetUserDBData().ChengeUserDBData(eUserDBData_Gold, -item.n32Cost);

                    } else {
                        return eEC_NotEnoughGold;
                    }
                } else {
                    return eEC_InvalidItemId;
                }
            }
            return 0;
        }
    }
}
