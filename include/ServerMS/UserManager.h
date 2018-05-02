#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "ServerMS/Log/Logging.h"
#include "ServerMS/ConfigManager.h"
#include "ServerMS/User.h"
#include "ServerMS/DB/DBManager.h"
#include "ServerMS/Network/Connection.h"
#include "ServerMS/DBActiveWrapper.h"
#include "ServerMS/ThreadSafeQueue.h"
#include "ServerMS/UserDBDataStruct.h"
#include "ServerMS/all_const.h"
#include "ServerMS/ParseProto.h"
#include "GCToGS.pb.h"
#include "GSToDB.pb.h"
#include "DBToGS.pb.h"
#include <sstream>

namespace ServerMS {

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

    class CKernel;

    class CUserManager : public boost::enable_shared_from_this<CUserManager>, private boost::noncopyable {
    public:
        CUserManager(boost::shared_ptr<CKernel> pKernel);

        bool Initialize();

        void RegisterMsgHandle(GCMsgHandlerMap &GCMsgHandlerMap);

        void ChangeUserNickName();

        void OnUsersUpdate();

        INT32 CombineGameUserID();

        INT32 CombineComputerID();

        INT32 GenerateItemDBID();

        DB::CDBConnector *GetDBSource(INT32 actorID);

        CDBActiveWrapper &GetNowWorkActor();

        void PostMsgToGC(INT32 n32ConnID, google::protobuf::MessageLite &rMsg, INT32 n32MsgID);
        void PostSaveCmd(); // empty 1
        bool UserAskLogin(GCToGS::AskLogin &rLogin, SUserNetInfo netinfo);

        void UserAskDBAsynHandler(CBuffer *pBuffer);

        void UserCacheDBAsynHandler(CBuffer *pBuffer);

        void DBAsyn_QueryWhenThreadBegin();

        void DBAsynUpdateUserCallback(CBuffer *pBuffer, DB::CDBConnector *pConnector);

        void DBAsynQueryUserCallback(CBuffer *pBuffer, DB::CDBConnector *pConnector);

        void DBAsynQueryUser(SUserDBData &sUserDBData, DBToGS::QueryUser &sQueryUser, DB::CDBConnector *pConnector);

        INT32 AddUser(UserPtr pUser);

        INT32 RemoveUser(UserPtr pUser);

        INT32 OnUserOnline(UserPtr pcUser, const SUserNetInfo &crsUserNetInfo);

        INT32 OnUserOffline(UserPtr pcUser);

        void DBAsyn_QueryUserComputers(DB::CDBConnector *pConnector, INT32 n32UserID, DBToGS::QueryUser &sQueryUser);

        void DBAsyn_QueryUserItems(DB::CDBConnector *pConnector, INT32 n32ComputerId, ComputerInfo *pMsgCompInfo);

        void DBAsyn_ExeSQL(CBuffer *pBuffer, DB::CDBConnector *pConnector);

        void DBAsynQueryUserCallBack();

        void SynUserAskDBCallBack();

        void SynHandleQueryUserCallback(CBuffer *pBuffer);

        bool DBPoster_UpdateUser(UserPtr pUser);

        void SynHandleAllAccountCallback(CBuffer *pBuffer);

        const boost::shared_ptr<CConfigManager> &GetConfMgr() { return m_pKernel->GetConfMgr(); }

        const boost::shared_ptr<CShopManager> &GetShopMgr() { return m_pKernel->GetShopMgr(); }
        void InsertNewUserToDB(GCToGS::AskLogin &rLogin, UserPtr pUser, INT32 n32DBGameUserID);

        void UpdateUserItem(SItemRecord &rsItemRecord, eDBOperation operation);

        void UpdateUserComputer(SComputerInfo &rsComputerInfo, eDBOperation operation);

        void UpdateUserItemInInvenoty(UserPtr user, SItemRecord &item_record);

        void OnHeartBeatImmediately();

        void GPSThreadBeginCallback();

        UserPtr CheckAndGetUserByNetInfo(const SUserNetInfo netinfo);

        UserPtr GetUserByNetInfo(const SUserNetInfo netinfo);

        INT32 OnMSgFromGC_AskStartMining(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

        INT32 OnMSgFromGC_AskLogin(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

        INT32 OnMsgFromGC_AskBuyItem(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo);

        INT32 OnMSgFromGC_AskInstallCCInComp(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo);

        void EncodeAndSendToLogicThread(google::protobuf::MessageLite &rMsg, INT32 n32MsgId);

        bool PostMsgToGC_AskReturn(const SUserNetInfo netinfo, INT32 n32ProtocolId, INT32 n32RetFlag);

    private:

        void DBAsyn_QueryUserInvenory(DB::CDBConnector *pConnector, INT32 n32UserID,
                                      DBToGS::QueryUser &sQueryUser);

    private:
        INT32 m_n32MaxGameUserID;
        INT32 m_n32MaxComputerID;
        INT32 m_n32MaxItemDBID;
        TIME_MILSEC m_n64LastUpdateTime;
        DB::CDBConnector *m_pDBConnector;
        CDBActiveWrapper *m_pThreadDBWrapper;
        CDBActiveWrapper *m_UserCacheDBActiveWrapper;
        CDBActiveWrapper *m_LoginDBWrapper;
        std::queue<CBuffer *> m_DBCallbackQueue;
        std::map<SUserCombineKey, INT32> m_AllUserNameIDMap;
        CThreadSafeObjectPool<CBuffer> m_CallbackQueuePool;
        typedef std::map<INT32, UserPtr> UserMap;
        UserMap m_cUserGUIDMap;
        UserMap m_cUserOnlineMap;
        std::map<SUserNetInfo, UserPtr> m_cUserNetMap;
        std::stringstream m_SaveUserStream;
        boost::shared_ptr<CKernel> m_pKernel;
    };

}

#endif /* USERMANAGER_H */