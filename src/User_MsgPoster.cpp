#include "User.h"
#include "Connection.h"
#include "GSToGC.pb.h"

namespace MS {
    namespace Server {

        void CUser::PostMsgToGC(google::protobuf::MessageLite &rMsg, int n32MsgID) {
            CKernel::GetInstance().PostMsgToGC(m_sUserNetInfo.n32GCConnID, rMsg, n32MsgID);
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
    }
}