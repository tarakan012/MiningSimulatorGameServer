#ifndef USER_H
#define USER_H

#include "all_const.h"
#include "stdafx.h"
#include "UserDBDataStruct.h"
#include "Kernel.h"

#include "GCToGS.pb.h"

#include "google/protobuf/message.h"

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif

        class CUser {
        public:
            CUser();

            void LoadDBData(SUserDBData &sUserDBData);

            int GetGameUserID() { return m_sUserDBData.sPODUserDBData.n32DBId; };

            void OnHeartBeart(int64_t n64Milsec, int64_t n64TickSpawn);

            int KickOutOldUser();

            int StartMiningByComputer(GCToGS::AskStartMining &rsMsg);

            SUserNetInfo &GetUserNetInfo() { return m_sUserNetInfo; };

            void ClearNetInfo();

            void PostMsgToGC(google::protobuf::MessageLite &rMsg, int n32MsgID);

            void OnOnline(SUserNetInfo &sUserNetInfo);

            void OnOfline();

            void SetUserNetInfo(const SUserNetInfo &sUserNetInfo);

            void SynUserGameInfo();

            void SynPayInfo();

            SUserDBData &GetUserDBData() { return m_sUserDBData; }

            bool AskUpdateItem(GCToGS::AskUpdateItemRet &Msg, CConnection *pConn);

            bool CheckComputerById(int n32CompId);

            bool CheckIfEnoughPay(eUserDBData_Type eType, int n32Pay);

            int GetMiningGoldComputerById(int n32CompId);

        private:
            int64_t m_n64LastUpdateTime;
            int64_t m_tStartAccamulEnergy;
            SUserNetInfo m_sUserNetInfo;
            SUserDBData m_sUserDBData;
        };

#ifdef __cplusplus
        }
#endif
    }
}
#endif /* USER_H */

