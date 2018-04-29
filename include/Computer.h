//
// Created by cucaracha on 28.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_COMPUTER_H
#define MININGSIMULATORGAMESERVER_COMPUTER_H

#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "PreDefine.h"
#include "ComputerComponent.h"

#include <map>

namespace MS {
    namespace Server {

        class CUser;

        typedef boost::shared_ptr<CUser> UserPtr;

        class CComputer : public boost::enable_shared_from_this<CComputer> {
        public:
            enum eErrorCode {
                eNormal = 0,
                eEC_NotEnoughEnergy
            };

            CComputer(UserPtr User);

            std::tuple<INT32/*error*/, GOLD> StartMining();

            std::tuple<bool/*replace*/, boost::shared_ptr<SCCMatherboard>> ReplaceMatherbord(
                    boost::shared_ptr<SCCMatherboard> matherboard);

            bool IsMatherboard() { return m_psMatherboard == nullptr ? 1 : 0; }

            INT32 InstallCpu(
                    boost::shared_ptr<SCCCpu> cpu);

            INT32 InstallGpu(
                    boost::shared_ptr<SCCGpu> gpu);

            void SetDbId(INT32 dbid) { m_DBId = dbid; }

            INT32 GetDbId() { return m_DBId; }

        private:
            UserPtr GetOwner() { return m_pOwner; }

            ENERGY GetConsumEnergy() { return m_ConsumptionEnergy; }

            void UpdateMiningGold(GOLD gold) { m_MiningGoldOnes += gold; }

            bool CheckIfEnoughEnergy();

            GOLD GetMiningGold() { return m_MiningGoldOnes; }

        public:
            boost::shared_ptr<SCCMatherboard> m_psMatherboard;
            UserPtr m_pOwner;
            INT32 m_DBId;
            ENERGY m_ConsumptionEnergy{};
            GOLD m_MiningGoldOnes{};
            std::map<INT32/*dbid*/, boost::shared_ptr<SCCCpu>> m_CpuMap;
            std::map<INT32/*dbid*/, boost::shared_ptr<SCCGpu>> m_GpuMap;
        };

        typedef boost::shared_ptr<CComputer> ComputerPtr;

    }
}

#endif //MININGSIMULATORGAMESERVER_COMPUTER_H
