//
// Created by cucaracha on 29.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_INVENTORY_H
#define MININGSIMULATORGAMESERVER_INVENTORY_H

#include "ServerMS/PreDefine.h"
#include "ServerMS/UserNetInfo.h"
#include "ServerMS/all_const.h"

#include "ServerMS/UserDBDataStruct.h"
#include "ServerMS/Kernel.h"
#include "ServerMS/Computer.h"
#include "GCToGS.pb.h"

#include "google/protobuf/message.h"
#include <boost/shared_ptr.hpp>
#include "ServerMS/all_const.h"

namespace ServerMS {

    class CUser;

    typedef boost::shared_ptr<CUser> UserPtr;

    class CInventory : public boost::enable_shared_from_this<CInventory> {
    public:
        enum eErrorCode {
            eNormal = 0,

        };

        CInventory(UserPtr User);

        boost::shared_ptr<SItemRecord> GetItemByDBId(INT32 item_dbid);

        void InsertItem(const boost::shared_ptr<SItemRecord> &item);

    private:
        UserPtr GetOwner() { return m_pOwner; }

    public:
        std::map<INT32/*itemdbid*/, boost::shared_ptr<SItemRecord>> m_ItemMap;
        UserPtr m_pOwner;
    };

}

#endif //MININGSIMULATORGAMESERVER_INVENTORY_H