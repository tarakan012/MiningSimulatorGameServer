//
// Created by cucaracha on 29.04.18.
//

#include "ServerMS/Inventory.h"

namespace ServerMS {

    CInventory::CInventory(UserPtr User)
            : m_pOwner{User} {

    }

    boost::shared_ptr<SItemRecord> CInventory::GetItemByDBId(INT32 item_dbid) {
        boost::shared_ptr<SItemRecord> item = nullptr;
        auto iter = m_ItemMap.find(item_dbid);
        if (iter != m_ItemMap.end()) {
            item = iter->second;
            m_ItemMap.erase(iter);
        }
        return item;
    }

    void CInventory::InsertItem(const boost::shared_ptr<SItemRecord> &item) {
        m_ItemMap[item->n32DBId] = item;
    }

}