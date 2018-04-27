#ifndef USER_H
#define USER_H

#include "PreDefine.h"
#include "UserNetInfo.h"
#include "all_const.h"

#include "UserDBDataStruct.h"
#include "Kernel.h"

#include "GCToGS.pb.h"

#include "google/protobuf/message.h"

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif
        //struct SUserNetInfo;
        class CUser {
        public:
            CUser();

            void LoadDBData(SUserDBData &sUserDBData);

            INT32 GetGameUserID() { return m_sUserDBData.sPODUserDBData.n32DBId; };

            void OnHeartBeart(TIME_MILSEC n64Milsec, TIME_MILSEC n64TickSpawn);

            INT32 KickOutOldUser();

            INT32 StartMiningByComputer(GCToGS::AskStartMining &rsMsg);

            SUserNetInfo &GetUserNetInfo() { return m_sUserNetInfo; };

            void ClearNetInfo();

            void PostMsgToGC(google::protobuf::MessageLite &rMsg, INT32 n32MsgID);

            void OnOnline(SUserNetInfo &sUserNetInfo);

            void OnOfline();

            void SetUserNetInfo(const SUserNetInfo &sUserNetInfo);

            void SynUserGameInfo();

            void SynShopPriceList();

            void SynPayInfo();

            SUserDBData &GetUserDBData() { return m_sUserDBData; }

            INT32 GetGold() { return m_sUserDBData.sPODUserDBData.n32Gold; }

            bool AskUpdateItem(GCToGS::AskUpdateItemRet &Msg, CConnection *pConn);

            bool CheckComputerById(INT32 n32CompId);

            bool CheckIfEnoughPay(eUserDBData_Type eType, INT32 n32Pay);

            INT32 GetMiningGoldComputerById(INT32 n32CompId);

        private:
            TIME_MILSEC m_n64LastUpdateTime;
            TIME_MILSEC m_tStartAccumEnergy;
            SUserNetInfo m_sUserNetInfo;
            SUserDBData m_sUserDBData;
        };

#ifdef __cplusplus
        }
#endif
    }
}
#endif /* USER_H */

