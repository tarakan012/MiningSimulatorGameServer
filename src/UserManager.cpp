#include "UserManager.h"
#include "DBManager.h"
#include <stdio.h>
#include "GCToGS.pb.h"

#include <strstream>
#include <GSToGC.pb.h>
#include "Time.h"

namespace MS {
    namespace Server {

        static CUserManager *pUMInstance = nullptr;

        CUserManager &CUserManager::GetInstance() {
            if (NULL == pUMInstance) {
                pUMInstance = new CUserManager;
            }
            return *pUMInstance;
        }

        CUserManager::CUserManager() {
            m_n64LastUpdateTime = 0;
        }

        void CUserManager::OnHeartBeatImmediately() {

            OnUsersUpdate();
            SynUserAskDBCallBack();
        }

        void CUserManager::OnUsersUpdate() {
            TIME_MILSEC n64TimeNow = GetTimeMillis();
            TIME_MILSEC n64TickSpawn = m_n64LastUpdateTime - m_n64LastUpdateTime;
            m_n64LastUpdateTime = n64TimeNow;

            for (auto const &user : m_cUserOnlineMap) {
                CUser *pUser = user.second;
                pUser->OnHeartBeart(n64TimeNow, n64TickSpawn);
            }
        }

        INT32  CUserManager::AddUser(CUser *pUser) {
            unsigned un32ObjIndex = pUser->GetGameUserID();
            m_cUserGUIDMap.insert(std::make_pair(un32ObjIndex, pUser));
            return 0;
        }

        INT32 CUserManager::RemoveUser(CUser *pUser) {
            DBPoster_UpdateUser(pUser);
            m_cUserGUIDMap.erase(pUser->GetGameUserID());
            delete pUser;
            return 0;
        }

        CUser *CUserManager::GetUserByNetInfo(const SUserNetInfo netinfo) {
            CUser *pcUser = NULL;
            auto iterUser = m_cUserNetMap.find(netinfo);
            if (m_cUserNetMap.end() != iterUser) {
                pcUser = iterUser->second;
            }
            return pcUser;
        }

        CUser *CUserManager::CheckAndGetUserByNetInfo(const SUserNetInfo netinfo) {
            CUser *pcUser = GetUserByNetInfo(netinfo);
            if (pcUser == NULL) {
                LogPrint(LogFlags::ALL, "CheckGetUserByNetInfo failed\n");

            }
            return pcUser;

        }

        INT32 CUserManager::OnUserOnline(CUser *pcUser, const SUserNetInfo &crsUserNetInfo) {
            m_cUserNetMap.insert(std::make_pair(crsUserNetInfo, pcUser));
            m_cUserOnlineMap.insert(std::make_pair(pcUser->GetGameUserID(), pcUser));
            pcUser->SetUserNetInfo(crsUserNetInfo);
            return 0;
        }

        INT32 CUserManager::OnUserOffline(CUser *pcUser) {
            m_cUserNetMap.erase(pcUser->GetUserNetInfo());

            m_cUserOnlineMap.erase(pcUser->GetGameUserID());
            pcUser->ClearNetInfo();
            return 0;
        }

        void CUserManager::EncodeAndSendToLogicThread(google::protobuf::MessageLite &rMsg, INT32 n32MsgId) {
            CBuffer *pBuffer = m_CallbackQueuePool.AcquireObject();
            CDBActiveWrapper::EncodeProtoMsgToBuffer(rMsg, n32MsgId, pBuffer);
            m_DBCallbackQueue.push(pBuffer);
        }


        bool CUserManager::Initialize() {
            m_UserCacheDBActiveWrapper = new CDBActiveWrapper(
                    std::bind(&CUserManager::UserCacheDBAsynHandler, this, std::placeholders::_1),
                    std::bind(&CUserManager::DBAsyn_QueryWhenThreadBegin, this));
            m_UserCacheDBActiveWrapper->Start();
            m_LoginDBWrapper = new CDBActiveWrapper(
                    std::bind(&CUserManager::UserAskDBAsynHandler, this, std::placeholders::_1),
                    std::bind(&CUserManager::GPSThreadBeginCallback, this));
            m_LoginDBWrapper->Start();
            m_n32MaxGameUserID = 0;
            m_n32MaxComputerID = 0;
            m_n32MaxItemDBID = 0;
        }

        INT32 CUserManager::CombineGameUserID() {
            m_n32MaxGameUserID++;
            return m_n32MaxGameUserID;
        }

        INT32 CUserManager::CombineComputerID() {
            m_n32MaxComputerID++;
            return m_n32MaxComputerID;
        }

        INT32 CUserManager::GenerateItemDBID() {
            m_n32MaxItemDBID++;
            return m_n32MaxItemDBID;
        }

        CDBConnector *CUserManager::GetDBSource(INT32 actorID) {
            return m_LoginDBWrapper->GetDBConnector();
        }

        CDBActiveWrapper &CUserManager::GetNowWorkActor() {
            return *m_LoginDBWrapper;
        }

        void CUserManager::DBAsyn_ExeSQL(CBuffer *pBuffer, CDBConnector *pConnector) {
            GSToDB::ExeSQL_Call exeSql;
            INT32 n32Result = exeSql.ParsePartialFromArray(pBuffer->GetCurData(), pBuffer->GetDataLength());
            if (!n32Result) return;
            pConnector->ExecQuery(exeSql.sql());
            pConnector->CloseQuery();
        }

#define GCCallback(Callback) std::bind(Callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        void CUserManager::RegisterMsgHandle(GCMsgHandlerMap &GCMsgHandlerMap) {
            GCMsgHandlerMap[GCToGS::eMsgToGSFromGC_AskLogin] = GCCallback(&CUserManager::OnMSgFromGC_AskLogin)
            GCMsgHandlerMap[GCToGS::eMsgToGSFromGC_AskStartMining] = GCCallback(
                                                                             &CUserManager::OnMSgFromGC_AskStartMining)
            GCMsgHandlerMap[GCToGS::eMsgToGSFromGC_AskBuyItem] = GCCallback(
                                                                             &CUserManager::OnMsgFromGC_AskBuyItem)
        }


        bool CUserManager::DBSynQueryAskUserItem(INT32 n32UserId, INT32 n32ItemId) {
            std::stringstream ss;
            ss << "select * from account where id=" << n32UserId;
            std::string str = ss.str();
        }

        void CUserManager::InsertNewUserToDB(GCToGS::AskLogin &rLogin, CUser *pUser, INT32 n32DBGameUserID) {
            GSToDB::ExeSQL_Call exeSql;
            std::stringstream strSql;

            strSql << "insert into account_user(id,name_googleplus,id_gps) values("
                   << n32DBGameUserID << ",'" << rLogin.namegoogleplus() << "'," << rLogin.id_gps() << ")";
            exeSql.set_sql(strSql.str());
            m_LoginDBWrapper->EncodeAndSendToDBThread(exeSql, exeSql.id());
            {
                GSToDB::ExeSQL_Call exeSql;
                std::stringstream strSql;
                strSql << "insert into game_user(id, btc, comp_id) values(" << n32DBGameUserID << ",0,'{";

                auto &rsUserCompInfo = pUser->GetUserDBData().CompInfoMap;
                INT32 counter = 0;
                INT32 n32NumComp = rsUserCompInfo.size();
                for (auto &comp : rsUserCompInfo) {
                    counter++;
                    SComputerInfo &rCompInfo = comp.second;
                    rCompInfo.n32DBId = CombineComputerID();
                    strSql << rCompInfo.n32DBId;
                    if (counter != n32NumComp) {
                        strSql << ",";
                    }
                }
                strSql << "}')";
                LogPrint(LogFlags::ALL, "sql %s", strSql.str());
                exeSql.set_sql(strSql.str());
                m_LoginDBWrapper->EncodeAndSendToDBThread(exeSql, exeSql.id());
            }
            {
                GSToDB::ExeSQL_Call exeSql;
                std::stringstream strSql;
                auto &rsUserCompInfo = pUser->GetUserDBData().CompInfoMap;

                for (auto &comp: rsUserCompInfo) {
                    SComputerInfo &rCompInfo = comp.second;
                    UpdateUserComputer(rCompInfo, eDBOperation::eOperationTypeAdd);
                }
            }
        }

        void CUserManager::SynHandleAllAccountCallback(CBuffer *pBuffer) {
            DBToGS::QueryAllAccount queryAllAccount;
            INT32 n32Result = queryAllAccount.ParseFromArray(pBuffer->GetCurData(), pBuffer->GetDataLength());
            if (!n32Result) return;
            for (INT32 i = 0; i < queryAllAccount.account_size(); i++) {
                auto pAccount = queryAllAccount.account(i);
                SUserCombineKey sUserCombineKey;
                sUserCombineKey.m_nameGooglePlus = pAccount.namegoogleplus();
                sUserCombineKey.m_id_gps = pAccount.id_gps();
                m_AllUserNameIDMap.insert(std::make_pair(sUserCombineKey, pAccount.id()));
                if (m_n32MaxGameUserID < pAccount.id()) {
                    m_n32MaxGameUserID = pAccount.id();
                }
            }
            LogPrint(LogFlags::ALL, "MaxGameUserID : %d\n", m_n32MaxGameUserID);

        }

        void CUserManager::UpdateUserComputer(SComputerInfo &rsComputerInfo, eDBOperation operation) {
            GSToDB::ExeSQL_Call exeSql;
            switch (operation) {
                case eDBOperation::eOperationTypeAdd : {
                    std::stringstream strSql;
                    strSql << "insert into computer_user(id,item_id,mining_gold) values(";
                    strSql << rsComputerInfo.n32DBId << ", '{";

                    INT32 n32CountDelim = rsComputerInfo.ItemRecordMap.size() - 1;
                    INT32 genItemID = 0;
                    INT32 ln32MiningGoldComp = 0;
                    for (auto &item : rsComputerInfo.ItemRecordMap) {
                        SItemRecord &rItemRecord = item.second;
                        ln32MiningGoldComp += rItemRecord.n32MiningGold;
                        genItemID = GenerateItemDBID();
                        rItemRecord.n32DBId = genItemID;
                        UpdateUserItem(rItemRecord, eDBOperation::eOperationTypeAdd);
                        strSql << genItemID;
                        if (n32CountDelim > 0) {
                            strSql << ",";
                            n32CountDelim--;
                        }
                    }
                    rsComputerInfo.n32MiningGold = ln32MiningGoldComp;
                    strSql << "}'," << ln32MiningGoldComp << ")";
                    exeSql.set_sql(strSql.str());
                    LogPrint(LogFlags::ALL, "sql %s", strSql.str());

                }
            }
            m_LoginDBWrapper->EncodeAndSendToDBThread(exeSql, exeSql.id());
        }

        void CUserManager::UpdateUserItem(SItemRecord &rsItemRecord, eDBOperation operation) {
            GSToDB::ExeSQL_Call exeSql;
            switch (operation) {
                case eDBOperation::eOperationTypeAdd : {

                    std::stringstream strSql;
                    strSql << "insert into item_user(id,name,mining_gold,item_id ) values(" << rsItemRecord.n32DBId
                           << ",'" << rsItemRecord.szName << "'," << rsItemRecord.n32MiningGold << ","
                           << rsItemRecord.n32Id << ")";
                    exeSql.set_sql(strSql.str());
                }
            }
            m_LoginDBWrapper->EncodeAndSendToDBThread(exeSql, exeSql.id());

        }

        bool CUserManager::PostMsgToGC_AskReturn(const SUserNetInfo netinfo, INT32 n32ProtocolId, INT32 n32RetFlag) {
            GSToGC::AskRet msg;
            msg.set_askid(n32ProtocolId);
            msg.set_errorcode(n32RetFlag);
            CKernel::GetInstance().PostMsgToGC(netinfo.n32GCConnID, msg, msg.msgid());
            return false;
        }
    }
}