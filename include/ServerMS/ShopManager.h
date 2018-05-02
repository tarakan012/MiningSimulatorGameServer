//
// Created by cucaracha on 27.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_SHOPMANAGER_H
#define MININGSIMULATORGAMESERVER_SHOPMANAGER_H

#include "ServerMS/Singleton.h"
#include "ServerMS/User.h"

namespace ServerMS {

    class CUserManager;
    class CShopManager {
    public:
        CShopManager(boost::shared_ptr<CUserManager> &rUsrMgr);
        INT32 Buy(UserPtr user, INT32 item_id);

    private:
        boost::shared_ptr<CUserManager> &m_rUsrMgr;
    };

}

#endif //MININGSIMULATORGAMESERVER_SHOPMANAGER_H