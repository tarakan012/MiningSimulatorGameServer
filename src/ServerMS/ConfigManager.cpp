#include "ServerMS/ConfigManager.h"
#include "Json.h"
#include "ServerMS/ConfigConst.h"
#include <fstream>

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
        file.open("../ThirdFunc/ConfigFile/ShopCfg.json");
        if (!file.is_open()) {
            return false;
        }
        file.seekg(0, file.end);
        INT32 length = file.tellg();
        file.seekg(0, file.beg);
        CHAR *pBuffer = new CHAR[length];
        file.read(pBuffer, length);

        Json *json = Json_create(pBuffer);
        delete[]pBuffer;
        Json *obj = json->child;
        for (INT32 i = 0; i < json->size; ++i) {
            std::map<INT32, SItemRecord> PageShopMap;
            INT32 n32PageType = Json_getInt(obj, "typePage", -1);

            Json *jsonArray = Json_getItem(obj, "itemIdPage");

            Json *JsonNumber = jsonArray->child;
            for (INT32 i = 0; jsonArray->size; ++i) {
                INT32 n32ItemId = JsonNumber->valueInt;
                auto iter = m_ItemRecordMap.find(n32ItemId);
                if (iter != m_ItemRecordMap.end()) {
                    PageShopMap[n32ItemId] = iter->second;
                } else {
                    LogPrint(LogFlags::CFG, "The wrong ItemID - %d\n", n32ItemId);
                    return false;
                }
                if (!JsonNumber->next) {
                    break;
                }
                JsonNumber = JsonNumber->next;
            }
            m_ShopCfgMap[n32PageType] = PageShopMap;
            if (!obj->next) {
                break;
            }
            obj = obj->next;
        }

        Json_dispose(json);
        file.close();
        return true;

    }

    bool CConfigManager::LoadItemCfg() {
        m_ItemRecordMap.clear();
        std::ifstream file;
        file.open("../ThirdFunc/ConfigFile/ItemCfg.json");
        if (!file.is_open()) {
            return false;
        }
        file.seekg(0, file.end);
        INT32 length = file.tellg();
        file.seekg(0, file.beg);
        CHAR *pBuffer = new CHAR[length];
        file.read(pBuffer, length);

        Json *json = Json_create(pBuffer);
        delete[]pBuffer;
        Json *obj = json->child;
        for (INT32 i = 0; i < json->size; ++i) {
            SItemRecord item;
            const CHAR *nameItem = Json_getString(obj, "name", "");
            std::memcpy(&item.szName, nameItem, std::strlen(nameItem));
            item.n32Id = Json_getInt(obj, "id", -1);
            item.n32Type = Json_getInt(obj, "type", -1);
            item.n32Lvl = Json_getInt(obj, "lvl", -1);
            item.n32Icon = Json_getInt(obj, "icon", -1);
            item.n32MiningGold = Json_getInt(obj, "miningGold", -1);
            item.n32MiningResourses = Json_getInt(obj, "miningResourses", -1);
            item.n32PowerConsumption = Json_getInt(obj, "powerConsumption", -1);
            item.n32ExpansionSlot = Json_getInt(obj, "expansionSlot", -1);
            item.n32LvlGrind = Json_getInt(obj, "lvlGrind", -1);
            item.n32Cost = Json_getInt(obj, "cost", -1);
            item.n32Strength = Json_getInt(obj, "strength", -1);
            m_ItemRecordMap[item.n32Id] = item;

            if (!obj->next) {
                break;
            }
            obj = obj->next;
        }

        Json_dispose(json);
        file.close();
        return true;
    }

    bool CConfigManager::LoadStartSetCfg() {
        std::ifstream file;
        file.open("../ThirdFunc/ConfigFile/StartSetCfg.json");
        if (!file.is_open()) {
            return false;
        }
        m_sStartSetCfg.sPODUserDBData.n32Energy = nMAX_ENERGY;
        file.seekg(0, file.end);
        INT32 length = file.tellg();
        file.seekg(0, file.beg);
        CHAR *pBuffer = new CHAR[length];
        file.read(pBuffer, length);

        Json *json = Json_create(pBuffer);
        delete[]pBuffer;
        Json *obj = json->child;

        for (INT32 numcomp = 0; numcomp < json->size; ++numcomp) {
            Json *jsonArray = Json_getItem(obj, "list_id");
            Json *JsonNumber = jsonArray->child;
            for (INT32 numitem = 0; numitem < jsonArray->size; ++numitem) {
                INT32 n32ItemID = JsonNumber->valueInt;
                auto iter = m_ItemRecordMap.find(n32ItemID);
                if (iter != m_ItemRecordMap.end()) {
                    m_sStartSetCfg.CompInfoMap[numcomp].ItemRecordMap[numitem] = iter->second;
                } else {
                    LogPrint(LogFlags::CFG, "The wrong ItemID - %d\n", n32ItemID);
                    return false;
                }
                if (!JsonNumber->next) {
                    break;
                }
                JsonNumber = JsonNumber->next;
            }
        }
        Json_dispose(json);
        file.close();
        return true;
    }

}