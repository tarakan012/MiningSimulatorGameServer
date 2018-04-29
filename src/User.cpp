#include "User.h"
#include "UserManager.h"
#include "Connection.h"
#include "AllErrorCode.h"
#include "Time.h"
#include "ConfigConst.h"
#include "ShopManager.h"

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
            CUserManager::GetInstance().OnUserOnline(shared_from_this(), sUserNetInfo);
            SynUserGameInfo();
            SynShopPriceList();
        }


        void CUser::OnOfline() {
            CUserManager::GetInstance().OnUserOffline(shared_from_this());
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

        INT32 CUser::AskStartMining(GCToGS::AskStartMining &rsMsg) {
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

        INT32 CUser::AskInstallCCInComp(GCToGS::AskInstallCCInComp &rsMsg) {
            auto comp = GetComputerById(rsMsg.compid());
            if (comp == NULL) {
                return eEC_InvalidCompId;
            }
            auto item = m_pcInventory->GetItemByDBId(rsMsg.item_dbid());
            if (item == NULL) {
                return eEC_InvalidItemId;
            }

            switch (item->n32Type) {
                case eTypeItem_CPU: {
                    auto cpu = boost::static_pointer_cast<SCCCpu>(item);
                    comp->InstallCpu(cpu);
                    break;
                }
                case eTypeItem_GPU: {
                    auto gpu = boost::static_pointer_cast<SCCGpu>(item);
                    comp->InstallGpu(gpu);
                    break;
                }
                case eTypeItem_Matherboard: {
                    auto matherboard = boost::static_pointer_cast<SCCMatherboard>(item);
                    comp->ReplaceMatherbord(matherboard);
                    break;
                }
                default:
                    LogPrintDebug("Invalid Item Type");
                    break;
            }
            return 0;
        }

        INT32 CUser::AskBuyItem(GCToGS::AskBuyItem &rsMsg) {
            INT32 n32RetFlag = 0;
            n32RetFlag = CShopManager::GetInstance().Buy(shared_from_this(), rsMsg.item_id());
            if (n32RetFlag == eNormal) {
                SynInventory();
            }
            return n32RetFlag;
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

        ComputerPtr CUser::GetComputerById(INT32 compid) {
            ComputerPtr comp = nullptr;
            auto iter = m_CompMap.find(compid);
            if (iter != m_CompMap.end()) {
                comp = iter->second;
            }
            return comp;
        }
    }
}