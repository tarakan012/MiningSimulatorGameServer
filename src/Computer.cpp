//
// Created by cucaracha on 28.04.18.
//

#include "Computer.h"
#include "User.h"

namespace MS {
    namespace Server {

        CComputer::CComputer(UserPtr owner) : m_pOwner{owner} {

        }

        bool CComputer::CheckIfEnoughEnergy() {
            return GetOwner()->CheckIfEnoughPay(eUserDBData_Energy, GetRequiredEnergy());
        }

        std::tuple<INT32, GOLD> CComputer::StartMining() {
            if (!CheckIfEnoughEnergy()) {
                return std::make_tuple(eEC_NotEnoughEnergy, GOLD(0));
            } else {
                return std::make_tuple(eNormal, GetMiningGold());
            }
        }

        std::tuple<bool, boost::shared_ptr<SCCMatherboard>> CComputer::ReplaceMatherbord(
                boost::shared_ptr<SCCMatherboard> matherboard) {
            bool bReaplace = false;
            boost::shared_ptr<SCCMatherboard> psOldMatherboard = nullptr;
            if (!IsMatherboard()) {
                m_psMatherboard = matherboard;
            } else {
                psOldMatherboard = m_psMatherboard;
                m_psMatherboard = matherboard;
            }
            return std::make_tuple(bReaplace, psOldMatherboard);
        }

        INT32
        CComputer::InstallCpu(boost::shared_ptr<SCCCpu> cpu) {
            auto iter = m_CpuMap.find(cpu->n32DBId);
            if (iter != m_CpuMap.end()) {

            } else {
                m_CpuMap[cpu->n32DBId] = cpu;
                UpdateMiningGold(cpu->n32MiningGold);
            }
            return 0;
        }

        INT32
        CComputer::InstallGpu(boost::shared_ptr<SCCGpu> gpu) {

            auto iter = m_GpuMap.find(gpu->n32DBId);
            if (iter != m_GpuMap.end()) {

            } else {
                m_GpuMap[gpu->n32DBId] = gpu;
                UpdateMiningGold(gpu->n32MiningGold);
            }
            return 0;
        }


    }
}