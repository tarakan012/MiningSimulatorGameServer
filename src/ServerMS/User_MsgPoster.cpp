#include "ServerMS/User.h"
#include "GSToGC.pb.h"
#include "ServerMS/ConfigManager.h"
#include "ServerMS/UserManager.h"

namespace ServerMS {

    void CUser::PostMsgToGC(google::protobuf::MessageLite &rMsg, int n32MsgID) {
        m_pUsrMgr->PostMsgToGC(m_sUserNetInfo.n32GCConnID, rMsg, n32MsgID);
    }

    void CUser::SynUserGameInfo() {
        GSToGC::UserGameInfo sUserGameInfo;
        sUserGameInfo.set_gold(m_sUserDBData.sPODUserDBData.n32Gold);
        sUserGameInfo.set_energy(m_sUserDBData.sPODUserDBData.n32Energy);
        for (auto const &comp : m_sUserDBData.CompInfoMap) {
            auto const &cCompDB = comp.second;
            auto pNewComp = sUserGameInfo.add_comp();
            pNewComp->set_dbid(cCompDB.n32DBId);
            for (auto const &item : cCompDB.ItemRecordMap) {
                auto const &cItemDB = item.second;
                auto pNewItem = pNewComp->add_item();
                pNewItem->set_id(cItemDB.n32DBId);
            }
        }
        PostMsgToGC(sUserGameInfo, sUserGameInfo.msgid());
    }

    void CUser::SynPayInfo() {
        GSToGC::UserPayInfo sUserPayInfo;
        sUserPayInfo.set_gold(GetUserDBData().sPODUserDBData.n32Gold);
        sUserPayInfo.set_energy(GetUserDBData().sPODUserDBData.n32Energy);
        PostMsgToGC(sUserPayInfo, sUserPayInfo.msgid());
    }

    void CUser::SynShopPriceList() {
        GSToGC::SynShopPriceList msg;
        const auto &cShopCfg = m_pUsrMgr->GetConfMgr()->GetShopCfg();
        for (auto const &page_pair : cShopCfg) {
            auto pMsgPage = msg.add_page();
            const std::map<INT32, SItemRecord> &PageMap = page_pair.second;
            for (auto const &item_pair : PageMap) {
                const auto &item = item_pair.second;
                auto pMsgItem = pMsgPage->add_item();
                pMsgItem->set_id(item.n32Id);
                pMsgItem->set_name(item.szName);
                pMsgItem->set_mining_gold(item.n32MiningGold);
                pMsgItem->set_cost(item.n32Cost);
            }
        }
        PostMsgToGC(msg, msg.msgid());
    }

    void CUser::SynInventory() {
        GSToGC::SynInventory msg;
        for (auto &item_pair : GetUserDBData().Inventory) {
            auto &item = item_pair.second;
            auto pMsgItem = msg.add_item();
            pMsgItem->set_dbid(item.n32DBId);
            pMsgItem->set_name(item.szName);
            pMsgItem->set_mining_gold(item.n32MiningGold);
        }
        PostMsgToGC(msg, msg.msgid());
    }

}