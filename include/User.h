#ifndef USER_H
#define USER_H

#include "PreDefine.h"
#include "UserNetInfo.h"
#include "all_const.h"

#include "UserDBDataStruct.h"
#include "Kernel.h"
#include "Computer.h"
#include "Inventory.h"
#include "GCToGS.pb.h"

#include "google/protobuf/message.h"
#include <boost/shared_ptr.hpp>

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif
        //struct SUserNetInfo;
        class CUser : public boost::enable_shared_from_this<CUser>, private boost::noncopyable {
        public:
            CUser();

            void LoadDBData(SUserDBData &sUserDBData);

            INT32 GetGameUserID() { return m_sUserDBData.sPODUserDBData.n32DBId; };

            void OnHeartBeart(TIME_MILSEC n64Milsec, TIME_MILSEC n64TickSpawn);

            INT32 KickOutOldUser();

            INT32 AskStartMining(GCToGS::AskStartMining &rsMsg);

            INT32 AskBuyItem(GCToGS::AskBuyItem &rsMsg);

            INT32 AskInstallCCInComp(GCToGS::AskInstallCCInComp &rsMsg);

            SUserNetInfo &GetUserNetInfo() { return m_sUserNetInfo; };

            void ClearNetInfo();

            void PostMsgToGC(google::protobuf::MessageLite &rMsg, INT32 n32MsgID);

            void OnOnline(SUserNetInfo &sUserNetInfo);

            void OnOfline();

            void SetUserNetInfo(const SUserNetInfo &sUserNetInfo);

            void SynUserGameInfo();

            void SynShopPriceList();

            void SynPayInfo();

            void SynInventory();
            SUserDBData &GetUserDBData() { return m_sUserDBData; }

            INT32 GetGold() { return m_sUserDBData.sPODUserDBData.n32Gold; }

            bool AskUpdateItem(GCToGS::AskUpdateItemRet &Msg, CConnection *pConn);

            bool CheckComputerById(INT32 n32CompId);

            bool CheckIfEnoughPay(eUserDBData_Type eType, INT32 n32Pay);

            ComputerPtr GetComputerById(INT32 compid);
            INT32 GetMiningGoldComputerById(INT32 n32CompId);

            void InsertComputer(ComputerPtr comp) { m_CompMap[comp->GetDbId()] = comp; }

            void SetInventory(boost::shared_ptr<CInventory> inventory) { m_pcInventory = inventory; }

        private:
            TIME_MILSEC m_n64LastUpdateTime;
            TIME_MILSEC m_tStartAccumEnergy;
            SUserNetInfo m_sUserNetInfo;
            SUserDBData m_sUserDBData;
            std::map<INT32/*compid*/, ComputerPtr> m_CompMap;
            boost::shared_ptr<CInventory> m_pcInventory;
        };

        typedef boost::shared_ptr<CUser> UserPtr;

#ifdef __cplusplus
        }
#endif
    }
}
#endif /* USER_H */

