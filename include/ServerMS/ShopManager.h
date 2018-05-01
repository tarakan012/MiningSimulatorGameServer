//
// Created by cucaracha on 27.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_SHOPMANAGER_H
#define MININGSIMULATORGAMESERVER_SHOPMANAGER_H

#include "ServerMS/Singleton.h"
#include "ServerMS/User.h"

namespace ServerMS {

    class CShopManager {
    public:
    DECLARE_SINGLETON(CShopManager)

        INT32 Buy(UserPtr user, INT32 item_id);

    private:

    };

}

#endif //MININGSIMULATORGAMESERVER_SHOPMANAGER_H