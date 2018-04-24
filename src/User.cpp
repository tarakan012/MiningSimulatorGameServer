#include "User.h"
#include "UserManager.h"
#include "Connection.h"
#include "AllErrorCode.h"
#include "Time.h"
#include "ConfigConst.h"

namespace MS {
    namespace Server {
        CUser::CUser() {
            m_n64LastUpdateTime = 0;
            m_tStartAccumEnergy = 0;
        }

        void CUser::OnHeartBeart(TIME_MILSEC n64Milsec, TIME_MILSEC n64TickSpawn) {
            if (!GetUserDBData().EnergyIsFool() && ((n64Milsec / 1000 - m_tStartAccumEnergy) > tACCUM_ONE_ENERGY)) {
                GetUserDBData().ChengeUserDBData(eUserDBData_Energy, 1);
                m_tStartAccumEnergy = GetTimeSec();
                SynPayInfo();
            }
        }

        bool CUser::AskUpdateItem(GCToGS::AskUpdateItemRet &Msg, CConnection *pConn) {

        }

        void CUser::LoadDBData(SUserDBData &sUserDBData) {
            m_sUserDBData = sUserDBData;
        }

        void CUser::OnOnline(SUserNetInfo &sUserNetInfo) {
            KickOutOldUser();
            CUserManager::GetInstance().OnUserOnline(this, sUserNetInfo);
            SynUserGameInfo();
        }


        void CUser::OnOfline() {
            CUserManager::GetInstance().OnUserOffline(this);
        }

        void CUser::SetUserNetInfo(const SUserNetInfo &sUserNetInfo) {
            m_sUserNetInfo = sUserNetInfo;
        }

        void CUser::ClearNetInfo() {
            m_sUserNetInfo.Clear();
        }

        INT32 CUser::KickOutOldUser() {
            if (m_sUserNetInfo.IsValid()) {
                OnOfline();
            }
            return 0;
        }

        INT32 CUser::StartMiningByComputer(GCToGS::AskStartMining &rsMsg) {
            INT32 ln32ComputerId = rsMsg.compid();
            if (!CheckComputerById(ln32ComputerId)) {
                return eEC_InvalidCompId;
            }
            if (!CheckIfEnoughPay(eUserDBData_Energy, 1)) {
                return eEC_NotEnoughEnergy;
            }
            INT32 ln32MiningGold = GetMiningGoldComputerById(ln32ComputerId);

            GetUserDBData().ChengeUserDBData(eUserDBData_Gold, ln32MiningGold);
            GetUserDBData().ChengeUserDBData(eUserDBData_Energy, -1);
            m_tStartAccumEnergy = GetTimeSec();
            GetUserDBData().ChengeUserDBData(eUserDBData_LastTimeM, m_tStartAccumEnergy);
            SynPayInfo();
            return 0;
        }

        bool CUser::CheckIfEnoughPay(eUserDBData_Type eType, INT32 n32Pay) {
            switch (eType) {
                case eUserDBData_Gold : {
                    return GetUserDBData().sPODUserDBData.n32Gold >= n32Pay;
                    break;
                }
                case eUserDBData_Energy : {
                    return GetUserDBData().sPODUserDBData.n32Energy >= n32Pay;
                    break;
                }
            }
            return false;
        }

        bool CUser::CheckComputerById(INT32 n32CompId) {
            for (auto comp : m_sUserDBData.CompInfoMap) {
                auto compinf = comp.second;
                if (n32CompId == compinf.n32DBId) {
                    return true;
                }
            }
            return false;
        }

        INT32 CUser::GetMiningGoldComputerById(INT32 n32CompId) {
            INT32 ln32MiningGold = 0;
            for (auto const &comp : m_sUserDBData.CompInfoMap) {
                auto compinf = comp.second;
                if (n32CompId == compinf.n32DBId) {
                    for (auto item : compinf.ItemRecordMap) {
                        auto iteminfo = item.second;
                        ln32MiningGold += iteminfo.n32MiningGold;
                    }
                }
            }
            return ln32MiningGold;
        }
    }
}