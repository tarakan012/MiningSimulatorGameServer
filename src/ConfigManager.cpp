#include "ConfigManager.h"

#include "Json.h"

#include <fstream>

namespace MS {
    namespace Server {

        static CConfigManager *pIRMInstance = nullptr;

        CConfigManager &CConfigManager::GetInstance() {
            if (NULL == pIRMInstance) {
                pIRMInstance = new CConfigManager;
            }
            return *pIRMInstance;
        }

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
            int length = file.tellg();
            file.seekg(0, file.beg);
            char *pBuffer = new char[length];
            file.read(pBuffer, length);

            Json *json = Json_create(pBuffer);
            delete[]pBuffer;
            Json *obj = json->child;
            for (int i = 0; i < json->size; ++i) {
                SShopCfg tempSCfg;
                tempSCfg.n32typePage = Json_getInt(obj, "typePage", -1);

                Json *jsonArray = Json_getItem(obj, "itemIdPage");

                Json *JsonNumber = jsonArray->child;
                for (int i = 0; jsonArray->size; ++i) {
                    tempSCfg.szItemId[i] = JsonNumber->valueInt;
                    if (!JsonNumber->next) {
                        break;
                    }
                    JsonNumber = JsonNumber->next;
                }
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
            int length = file.tellg();
            file.seekg(0, file.beg);
            char *pBuffer = new char[length];
            file.read(pBuffer, length);

            Json *json = Json_create(pBuffer);
            delete[]pBuffer;
            Json *obj = json->child;
            for (int i = 0; i < json->size; ++i) {
                SItemRecord item;
                const char *nameItem = Json_getString(obj, "name", "");
                std::memcpy(&item.szName, nameItem, std::strlen(nameItem));
                item.n32Id = Json_getInt(obj, "id", -1);
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
            m_sStartSetCfg.sPODUserDBData.n32Energy = 10;
            file.seekg(0, file.end);
            int length = file.tellg();
            file.seekg(0, file.beg);
            char *pBuffer = new char[length];
            file.read(pBuffer, length);

            Json *json = Json_create(pBuffer);
            delete[]pBuffer;
            Json *obj = json->child;

            for (int numcomp = 0; numcomp < json->size; ++numcomp) {
                Json *jsonArray = Json_getItem(obj, "list_id");
                Json *JsonNumber = jsonArray->child;
                for (int numitem = 0; numitem < jsonArray->size; ++numitem) {
                    int n32ItemID = JsonNumber->valueInt;
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
}
