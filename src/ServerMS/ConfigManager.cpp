#include "ServerMS/ConfigManager.h"

#include <fstream>
#include "fast-cpp-csv-parser/csv.h"
#include "nlohmann/json.hpp"

using namespace Log;

namespace ServerMS {

    bool CConfigManager::Initialize() {
        bool bLoadRet = true;
        if (bLoadRet) bLoadRet = LoadItemCfg();
        if (bLoadRet) bLoadRet = LoadShopCfg();
        if (bLoadRet) bLoadRet = LoadStartSetCfg();
        return bLoadRet;
    }

    bool CConfigManager::LoadShopCfg() {
        m_ShopCfgMap.clear();
        std::ifstream file;
        nlohmann::json json;
        file.open("../ThirdFunc/ConfigFile/ShopCfg.json");
        if (!file.is_open()) {
            return false;
        }
        json << file;
        for (auto element : json) {
            nlohmann::json page_shop(element);
            INT32 PageNumber = page_shop.at("PageNumber").get<INT32>();
            INT32 Type = page_shop.at("Type").get<INT32>();
            nlohmann::json ItemIDArray = page_shop.at("ItemIDArray");
            std::map<INT32, SItemRecord> PageShopMap;
            for (auto item_id : ItemIDArray) {
                auto iter = m_ItemRecordMap.find(item_id);
                if (iter != m_ItemRecordMap.end()) {
                    PageShopMap[item_id] = iter->second;
                } else {
                    LogPrint(LogFlags::CFG, "The wrong ItemID - %d\n", item_id);
                    return false;
                }
            }
            m_ShopCfgMap[Type] = PageShopMap;
        }
        return true;
    }

    bool CConfigManager::LoadItemCfg() {
        m_ItemRecordMap.clear();

        io::CSVReader<11> reader("../ThirdFunc/ConfigFile/ItemConfig.csv");

        reader.read_header(io::ignore_extra_column, "id", "name", "type", "lvl", "icon_id", "mining_gold",
                           "mining_resources", "energy_consumption", "strength", "cost", "number_slots");
        SItemRecord item;

        while (reader.read_row(item.n32Id,
                               item.NameStr,
                               item.n32Type,
                               item.n32Lvl,
                               item.n32Icon,
                               item.n32MiningGold,
                               item.n32MiningResourses,
                               item.n32EnergyConsumption,
                               item.n32Strength,
                               item.n32Cost,
                               item.n32NumberSlots)) {
            m_ItemRecordMap[item.n32Id] = item;
        }

        return true;
    }

    bool CConfigManager::LoadStartSetCfg() {
        std::ifstream file;
        nlohmann::json json;
        file.open("../ThirdFunc/ConfigFile/StartSetCfg.json");
        if (!file.is_open()) {
            return false;
        }
        m_sStartSetCfg.sPODUserDBData.n32Energy = nMAX_ENERGY;
        json << file;
        for (auto element : json) {
            nlohmann::json computer(element);
            INT32 CompID = computer.at("CompID").get<INT32>();
            nlohmann::json ItemIDArray = computer.at("ItemIDArray");
            std::map<INT32, SItemRecord> PageShopMap;
            for (auto item_id : ItemIDArray) {
                auto iter = m_ItemRecordMap.find(item_id);
                if (iter != m_ItemRecordMap.end()) {
                    m_sStartSetCfg.CompInfoMap[CompID].ItemRecordMap[item_id] = iter->second;
                } else {
                    LogPrint(LogFlags::CFG, "The wrong ItemID - %d\n", item_id);
                    return false;
                }
            }
        }
        return true;
    }

}