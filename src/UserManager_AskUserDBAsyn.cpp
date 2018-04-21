#include "UserManager.h"

#include "boost/regex.hpp"

namespace MS {
    namespace Server {

        void CUserManager::UserAskDBAsynHandler(CBuffer *pBuffer) {
            LogPrint(LogFlags::ALL, "log UserAskDBAsynHandler\n");
            CDBConnector *pConnector = GetDBSource(pBuffer->m_ActorID);
            if (!pConnector) {
                LogPrint(LogFlags::ALL, "DBConnector NULL\n");
                return;
            }
            switch (pBuffer->m_LogLvl) {
                case GSToDB::eExeSQL_Call : {
                    DBAsyn_ExeSQL(pBuffer, pConnector);
                    break;
                }
                case GSToDB::eQueryUser_DBCallBack : {
                    DBAsynQueryUserCallback(pBuffer, pConnector);
                    break;

                }
                default : {
                    LogPrint(LogFlags::ALL, "Unknown Msg\n");
                    break;
                }
            }
        }


        void CUserManager::DBAsynQueryUser(SUserDBData &sUserDBData, DBToGS::QueryUser &sQueryUser,
                                           CDBConnector *pConnector) {
            std::stringstream strSql;
            int guid = sUserDBData.sPODUserDBData.n32DBId;
            strSql << "select btc from game_user where id=" << guid;
            pConnector->ExecQuery(strSql.str());
            pConnector->GetQueryFieldData("btc", sUserDBData.sPODUserDBData.n32Gold);
            pConnector->CloseQuery();
            DBAsyn_QueryUserComputers(pConnector, guid, sQueryUser);
            for (int i = 0; i < sQueryUser.computer_size(); ++i) {
                auto comp = sQueryUser.mutable_computer(i);
                sUserDBData.sPODUserDBData.n32Gold += comp->bufgold();
            }
            sQueryUser.set_db((char *) &sUserDBData.sPODUserDBData, sizeof(sUserDBData.sPODUserDBData));
        }

        void CUserManager::DBAsyn_QueryUserComputers(CDBConnector *pConnector, int n32UserID,
                                                     DBToGS::QueryUser &sQueryUser) {
            std::stringstream strSql;
            std::string IdCompList;
            strSql << "select array_to_string(comp_id, ',') as idcomplist from game_user where id=" << n32UserID;
            pConnector->ExecQuery(strSql.str());
            pConnector->GetQueryFieldData("idcomplist", IdCompList);
            pConnector->CloseQuery();
            boost::regex delim("\\,");
            boost::sregex_token_iterator iter(IdCompList.begin(), IdCompList.end(), delim, -1);
            boost::sregex_token_iterator iter_end;

            while (iter != iter_end) {
                strSql.str(std::string());
                std::string strcompid = *iter++;
                int compid = atoi(strcompid.c_str());

                auto comp = sQueryUser.add_computer();
                comp->set_id(compid);
                strSql
                        << "select round(extract(epoch from now() - \"start_mining\"))::int as work_time from computer_user where id="
                        << strcompid;
                pConnector->ExecQuery(strSql.str());
                int worktime = 0;
                int mininggold = 0;
                int ln32BuffGold = 0;
                pConnector->GetQueryFieldData("work_time", worktime);
                comp->set_worktime(worktime);
                pConnector->CloseQuery();
                if (worktime > 0) {
                    strSql.str(std::string());

                    strSql << "select mining_gold from computer_user where id=" << strcompid;
                    pConnector->ExecQuery(strSql.str());
                    pConnector->GetQueryFieldData("mining_gold", mininggold);
                    comp->set_mininggold(mininggold);
                    pConnector->CloseQuery();
                }
                ln32BuffGold = mininggold * worktime;
                comp->set_bufgold(ln32BuffGold);
                LogPrint(LogFlags::ALL, "compid: %d, worktime: %d, mininggold: %d\n", compid, worktime, mininggold);
            }

        }

        void CUserManager::DBAsyn_QueryUserItems(CDBConnector *pConnector, int n32ComputerId,
                                                 DBToGS::QueryUser_ComputerInfo &rsComputerInfo) {
            std::string strItemsId;
            std::stringstream strSql;
            strSql << "select array_to_string(item_id,',') as iditemlist from computer_user where id=" << n32ComputerId;
            pConnector->ExecQuery(strSql.str());
            pConnector->GetQueryFieldData("idcomplist", strItemsId);
            pConnector->CloseQuery();
            boost::regex delim("\\,");
            boost::sregex_token_iterator iter(strItemsId.begin(), strItemsId.end(), delim, -1);
            boost::sregex_token_iterator iter_end;
            while (iter != iter_end) {
                strSql.str(std::string());
                std::string strItemId = *iter++;
                int l_n32ItemId = atoi(strItemId.c_str());
                auto item = rsComputerInfo.add_item();
                item->set_id(l_n32ItemId);
            }
        }
    }
}

