//
// Created by cucaracha on 26.04.18.
//

#include "ServerMS/ShopManager.h"
#include "ServerMS/ConfigManager.h"
#include "ServerMS/AllErrorCode.h"
#include "ServerMS/UserManager.h"

namespace ServerMS {

    CShopManager::CShopManager(boost::shared_ptr<CUserManager> &rUsrMgr)
            : m_rUsrMgr(rUsrMgr) {

    }

    INT32 CShopManager::Buy(UserPtr user, INT32 item_id) {
        auto &cShopCfg = m_rUsrMgr->GetConfMgr()->GetShopCfg();
        for (auto &page_pair : cShopCfg) {
            std::map<INT32, SItemRecord> &PageShopMap = page_pair.second;
            auto iter = PageShopMap.find(item_id);
            if (iter != PageShopMap.end()) {
                auto item = PageShopMap[item_id];
                if (user->GetGold() >= item.n32Cost) {
                    user->GetUserDBData().ChengeUserDBData(eUserDBData_Gold, -item.n32Cost);
                    INT32 item_dbid = m_rUsrMgr->GenerateItemDBID();
                    item.n32DBId = item_dbid;
                    user->GetUserDBData().Inventory[item.n32DBId] = item;
                    m_rUsrMgr->UpdateUserItemInInvenoty(user, item);
                    return eNormal;
                } else {
                    return eEC_NotEnoughGold;
                }
            } else {
                return eEC_InvalidItemId;
            }
        }
    }

}