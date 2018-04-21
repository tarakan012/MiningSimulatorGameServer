#include "User.h"
#include "UserManager.h"
#include "Connection.h"
#include "AllErrorCode.h"
#include "util_time.h"

namespace MS {
    namespace Server {
        CUser::CUser() {
            m_n64LastUpdateTime = 0;
            m_tStartAccamulEnergy = 0;
        }

        void CUser::OnHeartBeart(int64_t n64Milsec, int64_t n64TickSpawn) {
            if (!GetUserDBData().EnergyIsFool() && ((n64Milsec - m_tStartAccamulEnergy) > 30 * 1000)) {
                GetUserDBData().ChengeUserDBData(eUserDBData_Energy, 1);
                m_tStartAccamulEnergy = GetTimeMillis();
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

        int CUser::KickOutOldUser() {
            if (m_sUserNetInfo.IsValid()) {
                OnOfline();
            }
            return 0;
        }

        int CUser::StartMiningByComputer(GCToGS::AskStartMining &rsMsg) {
            int ln32ComputerId = rsMsg.compid();
            if (!CheckComputerById(ln32ComputerId)) {
                return eEC_InvalidCompId;
            }
            if (!CheckIfEnoughPay(eUserDBData_Energy, 1)) {
                return eEC_NotEnoughEnergy;
            }
            int ln32MiningGold = GetMiningGoldComputerById(ln32ComputerId);

            GetUserDBData().ChengeUserDBData(eUserDBData_Gold, ln32MiningGold);
            GetUserDBData().ChengeUserDBData(eUserDBData_Energy, -1);
            m_tStartAccamulEnergy = GetTimeMillis();
            SynPayInfo();
            return 0;
        }

        bool CUser::CheckIfEnoughPay(eUserDBData_Type eType, int n32Pay) {
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

        bool CUser::CheckComputerById(int n32CompId) {
            for (auto comp : m_sUserDBData.CompInfoMap) {
                auto compinf = comp.second;
                if (n32CompId == compinf.n32DBId) {
                    return true;
                }
            }
            return false;
        }

        int CUser::GetMiningGoldComputerById(int n32CompId) {
            int ln32MiningGold = 0;
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
