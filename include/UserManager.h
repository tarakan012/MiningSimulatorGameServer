#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "PreDefine.h"
#include "Logging.h"
#include "ConfigManager.h"
#include "stdafx.h"
#include "User.h"
#include "DBManager.h"
#include "Connection.h"
#include "UserNetInfo.h"
#include "DBActiveWrapper.h"
#include "ThreadSafeQueue.h"
#include "UserDBDataStruct.h"
#include "all_const.h"
#include "ParseProto.h"

#include "GCToGS.pb.h"
#include "GSToDB.pb.h"
#include "DBToGS.pb.h"

#include <queue>
#include <map>

#include "boost/shared_ptr.hpp"

#include "google/protobuf/util/json_util.h"
#include "google/protobuf/message.h"
#include <sstream>

namespace MS {
    namespace Server {


#ifdef __cplusplus
        extern "C" {
#endif

        struct SUserCombineKey {
            std::string m_nameGooglePlus;
            INT32 m_id_gps;

            SUserCombineKey() {}

            SUserCombineKey(const std::string &nameGooglePlus, INT32 id_gps) : m_nameGooglePlus(nameGooglePlus),
                                                                               m_id_gps(id_gps) {}

            bool operator<(const SUserCombineKey &sUserCombineKey) const {
                INT32 n32Result = m_nameGooglePlus.compare(sUserCombineKey.m_nameGooglePlus);
                if (n32Result == 0) {
                    return m_id_gps < sUserCombineKey.m_id_gps;
                }

                return n32Result < 0;
            }
        };

        class CUserManager {
        public:
            CUserManager();

            bool Initialize();

            static CUserManager &GetInstance();

            void RegisterMsgHandle(GCMsgHandlerMap &GCMsgHandlerMap);

            void ChangeUserNickName();

            void OnUsersUpdate();

            INT32 CombineGameUserID();

            INT32 CombineComputerID();

            INT32 GenerateItemDBID();

            CDBConnector *GetDBSource(INT32 actorID);

            CDBActiveWrapper &GetNowWorkActor();

            void PostSaveCmd(); // empty 1
            bool UserAskLogin(GCToGS::AskLogin &rLogin, SUserNetInfo netinfo);

            void UserAskDBAsynHandler(CBuffer *pBuffer);

            void UserCacheDBAsynHandler(CBuffer *pBuffer);

            void DBAsyn_QueryWhenThreadBegin();

            void DBAsynUpdateUserCallback(CBuffer *pBuffer, CDBConnector *pConnector);

            void DBAsynQueryUserCallback(CBuffer *pBuffer, CDBConnector *pConnector);

            void DBAsynQueryUser(SUserDBData &sUserDBData, DBToGS::QueryUser &sQueryUser, CDBConnector *pConnector);

            INT32 AddUser(CUser *pUser);

            INT32 RemoveUser(CUser *pUser);

            INT32 OnUserOnline(CUser *pcUser, const SUserNetInfo &crsUserNetInfo);

            INT32 OnUserOffline(CUser *pcUser);

            void DBAsyn_QueryUserComputers(CDBConnector *pConnector, INT32 n32UserID, DBToGS::QueryUser &sQueryUser);

            void DBAsyn_QueryUserItems(CDBConnector *pConnector, INT32 n32ComputerId, ComputerInfo *pMsgCompInfo);

            void DBAsyn_ExeSQL(CBuffer *pBuffer, CDBConnector *pConnector);

            void DBAsynQueryUserCallBack();

            void SynUserAskDBCallBack();

            void SynHandleQueryUserCallback(CBuffer *pBuffer);

            bool DBPoster_UpdateUser(CUser *pUser);

            void SynHandleAllAccountCallback(CBuffer *pBuffer);

            void InsertNewUserToDB(GCToGS::AskLogin &rLogin, CUser *pUser, INT32 n32DBGameUserID);

            void UpdateUserItem(SItemRecord &rsItemRecord, eDBOperation operation);

            void UpdateUserComputer(SComputerInfo &rsComputerInfo, eDBOperation operation);

            void UpdateUserItemInInvenoty(CUser *user, SItemRecord &item_record);
            void OnHeartBeatImmediately();

            void GPSThreadBeginCallback();

            CUser *CheckAndGetUserByNetInfo(const SUserNetInfo netinfo);

            CUser *GetUserByNetInfo(const SUserNetInfo netinfo);

            INT32 OnMSgFromGC_AskStartMining(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

            INT32 OnMSgFromGC_AskLogin(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

            INT32 OnMsgFromGC_AskBuyItem(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo);

            void EncodeAndSendToLogicThread(google::protobuf::MessageLite &rMsg, INT32 n32MsgId);

            bool PostMsgToGC_AskReturn(const SUserNetInfo netinfo, INT32 n32ProtocolId, INT32 n32RetFlag);

        private:

            void DBAsyn_QueryUserInvenory(CDBConnector *pConnector, INT32 n32UserID,
                                          DBToGS::QueryUser &sQueryUser);

        private:
            INT32 m_n32MaxGameUserID;
            INT32 m_n32MaxComputerID;
            INT32 m_n32MaxItemDBID;
            TIME_MILSEC m_n64LastUpdateTime;
            CDBConnector *m_pDBConnector;
            CDBActiveWrapper *m_pThreadDBWrapper;
            CDBActiveWrapper *m_UserCacheDBActiveWrapper;
            CDBActiveWrapper *m_LoginDBWrapper;
            std::queue<CBuffer *> m_DBCallbackQueue;
            std::map<SUserCombineKey, INT32> m_AllUserNameIDMap;
            CThreadSafeObjectPool<CBuffer> m_CallbackQueuePool;
            typedef std::map<INT32, CUser *> UserMap;
            UserMap m_cUserGUIDMap;
            UserMap m_cUserOnlineMap;
            std::map<SUserNetInfo, CUser *> m_cUserNetMap;
            std::stringstream m_SaveUserStream;
        };


#ifdef __cplusplus
        }
#endif

    }
}

#endif /* USERMANAGER_H */

