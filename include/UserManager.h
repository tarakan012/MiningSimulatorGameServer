#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "util.h"
#include "ConfigManager.h"
#include "stdafx.h"
#include "User.h"
#include "DBManager.h"
#include "Connection.h"
#include "GCInfo.h"
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

namespace MS {
    namespace Server{


#ifdef __cplusplus
extern "C" {
#endif

    struct SUserCombineKey{
		std::string m_nameGooglePlus;
		int m_id_gps;
		SUserCombineKey(){}
		SUserCombineKey(const std::string & nameGooglePlus, int id_gps):m_nameGooglePlus(nameGooglePlus), m_id_gps(id_gps){}
                bool operator <(const SUserCombineKey & sUserCombineKey) const
                {
                    int n32Result = m_nameGooglePlus.compare(sUserCombineKey.m_nameGooglePlus);
                    if(n32Result == 0)
                    {
                        return m_id_gps < sUserCombineKey.m_id_gps;
                    }

                    return n32Result < 0;
                }
	};

        class CUserManager
        {
        public:
            CUserManager();
            bool Initialize();
            static CUserManager & GetInstance();
            void RegisterMsgHandle(GCMsgHandlerMap & GCMsgHandlerMap);
            void ChangeUserNickName();
            void OnUsersUpdate();
            int CombineGameUserID();
            int CombineComputerID();
            int GenerateItemDBID();

            CDBConnector * GetDBSource(int actorID);
            CDBActiveWrapper & GetNowWorkActor();
            void PostSaveCmd(); // empty 1
            bool UserAskLogin(GCToGS::AskLogin & rLogin, CGCInfo * pGCInfo);
            void UserAskDBAsynHandler(CBuffer * pBuffer);
            void UserCacheDBAsynHandler(CBuffer * pBuffer);
            void DBAsyn_QueryWhenThreadBegin();
            void DBAsynUpdateUserCallback(CBuffer * pBuffer, CDBConnector *pConnector);
            void DBAsynQueryUserCallback(CBuffer * pBuffer, CDBConnector *pConnector);
            void DBAsynQueryUser(SUserDBData & sUserDBData, DBToGS::QueryUser & sQueryUser, CDBConnector *pConnector);
                    int AddUser(CUser * pUser);
                    int RemoveUser(CUser * pUser);
            int OnUserOnline(CUser *pcUser, const SUserNetInfo &crsUserNetInfo);
            int OnUserOffline(CUser *pcUser);

            void DBAsyn_QueryUserComputers(CDBConnector * pConnector, int n32UserID, DBToGS::QueryUser & sQueryUser);
            void DBAsyn_QueryUserItems(CDBConnector * pConnector, int n32ComputerId, DBToGS::QueryUser_ComputerInfo & rsComputerInfo);
            void DBAsyn_ExeSQL(CBuffer * pBuffer, CDBConnector *pConnector);
            void DBAsynQueryUserCallBack();
            void SynUserAskDBCallBack();
            void SynHandleQueryUserCallback(CBuffer * pBuffer);
            bool DBPoster_UpdateUser(CUser * pUser);// empty 1
            void SynHandleAllAccountCallback(CBuffer * pBuffer);
            void InsertNewUserToDB(GCToGS::AskLogin & rLogin, CUser * pUser, int n32DBGameUserID);
            void UpdateUserItem(SItemRecord & rsItemRecord, eDBOperation operation);
            void UpdateUserComputer(SComputerInfo & rsComputerInfo, eDBOperation operation);
            void OnHeartBeatImmediately();
            void GPSThreadBeginCallback();
            CUser * CheckAndGetUserByNetInfo(const CGCInfo * pGCInfo);
            CUser * GetUserByNetInfo(const CGCInfo * pGCInfo);
            int OnMSgFromGC_AskStartMining(const char * pMsg, int n32MsgLength, const CGCInfo * pGCInfo);
            int OnMSgFromGC_AskLogin(const char * pMsg, int n32MsgLength, CGCInfo * pGCInfo);
            int OnMsgFromGC_AskUpdateItemRet(const char * pMsg, int n32MsgLength, CConnection * pConn);
            bool DBSynQueryAskUserItem(int n32UserId, int n32ItemId);
            void EncodeAndSendToLogicThread(google::protobuf::Message & rMsg, int n32MsgId);
            bool PostMsgToGC_AskReturn(const CGCInfo * pGCInfo, int n32ProtocolId, int n32RetFlag);
        private:
            int m_n32MaxGameUserID;
            int m_n32MaxComputerID;
            int m_n32MaxItemDBID;
            int64_t m_n64LastUpdateTime;
            CDBConnector * m_pDBConnector;
            CDBActiveWrapper * m_pThreadDBWrapper;
            CDBActiveWrapper * m_UserCacheDBActiveWrapper;
            CDBActiveWrapper * m_LoginDBWrapper;
            std::queue<CBuffer*> m_DBCallbackQueue;
            std::map<SUserCombineKey,int> m_AllUserNameIDMap;
            CThreadSafeObjectPool<CBuffer> m_CallbackQueuePool;
            typedef	std::map<UINT64, CUser*>	UserMap;
            UserMap						m_cUserGUIDMap;
            UserMap						m_cUserOnlineMap;
            std::map<SUserNetInfo, CUser*>	m_cUserNetMap;
        };


#ifdef __cplusplus
}
#endif

    }
}

#endif /* USERMANAGER_H */

