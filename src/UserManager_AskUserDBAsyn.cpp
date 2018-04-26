#include "UserManager.h"
#include "Time.h"
#include "ConfigConst.h"
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
            INT32 guid = sUserDBData.sPODUserDBData.n32DBId;
            strSql << "select btc from game_user where id=" << guid;
            pConnector->ExecQuery(strSql.str());
            pConnector->GetQueryFieldData("btc", sUserDBData.sPODUserDBData.n32Gold);
            pConnector->CloseQuery();
            strSql.clear();
            strSql.str("");
            strSql << "select energy from game_user where id=" << guid;
            pConnector->ExecQuery(strSql.str());
            INT32 n32Energy = 0;
            pConnector->GetQueryFieldData("energy", n32Energy);
            sUserDBData.sPODUserDBData.n32Energy = n32Energy;
            pConnector->CloseQuery();
            if (!sUserDBData.EnergyIsFool()) {
                strSql.clear();
                strSql.str("");
                TIME_MILSEC tLastStartMining = 0;
                strSql << "select round(extract(epoch from \"last_start_mining\"))::int as last_start from game_user where id=" << guid;
                pConnector->ExecQuery(strSql.str());
                pConnector->GetQueryFieldData("last_start", tLastStartMining);
                pConnector->CloseQuery();
                if (tLastStartMining != 0) {
                    TIME_MILSEC tNow = GetTimeSec();
                    INT32 n32AccumEnergy = (tNow - tLastStartMining) / tACCUM_ONE_ENERGY;
                    if (n32AccumEnergy == 0) {
                        sUserDBData.sPODUserDBData.tLastStartMining = tLastStartMining;
                    } else {
                        n32AccumEnergy = std::min<INT32>(n32AccumEnergy,nMAX_ENERGY);
                        if((n32AccumEnergy + n32Energy) <= nMAX_ENERGY)
                        {
                            sUserDBData.ChengeUserDBData(eUserDBData_Energy, n32AccumEnergy);
                        } else
                        {
                            sUserDBData.ChengeUserDBData(eUserDBData_Energy, nMAX_ENERGY - n32Energy);
                        }
                    }
                }
            }
            DBAsyn_QueryUserComputers(pConnector, guid, sQueryUser);

            sQueryUser.set_db((CHAR *) &sUserDBData.sPODUserDBData, sizeof(sUserDBData.sPODUserDBData));
        }

        void CUserManager::DBAsyn_QueryUserComputers(CDBConnector *pConnector, INT32 n32UserID,
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
                INT32 compid = atoi(strcompid.c_str());
                auto comp = sQueryUser.add_computer();
                comp->set_id(compid);
                DBAsyn_QueryUserItems(pConnector, compid, comp);
//                strSql << "select round(extract(epoch from now() - \"last_start\"))::int as no_work_time from computer_user where id="
//                       << strcompid;
            }
        }

        void CUserManager::DBAsyn_QueryUserItems(CDBConnector *pConnector, INT32 n32ComputerId,
                                                 DBToGS::QueryUser_ComputerInfo *pMsgCompInfo) {
            std::string strItemsId;
            std::stringstream strSql;
            strSql << "select array_to_string(item_id,',') as iditemlist from computer_user where id=" << n32ComputerId;
            pConnector->ExecQuery(strSql.str());
            pConnector->GetQueryFieldData("iditemlist", strItemsId);
            pConnector->CloseQuery();
            boost::regex delim("\\,");
            boost::sregex_token_iterator iter(strItemsId.begin(), strItemsId.end(), delim, -1);
            boost::sregex_token_iterator iter_end;
            while (iter != iter_end) {
                strSql.clear();
                strSql.str(std::string());
                std::string strItemId = *iter++;
                INT32 l_n32ItemId = atoi(strItemId.c_str());
                auto item = pMsgCompInfo->add_item();
                item->set_id(l_n32ItemId);
                strSql << "select name, mining_gold, item_id from item_user where id=" << l_n32ItemId;
                pConnector->ExecQuery(strSql.str());
                INT32 mining_gold = 0;
                pConnector->GetQueryFieldData("mining_gold", mining_gold);
                item->set_mining_gold(mining_gold);
                INT32 item_id = 0;


                std::string name("");
                pConnector->GetQueryFieldData("name", name);
                item->set_name(name);
                pConnector->CloseQuery();
            }
        }
    }
}