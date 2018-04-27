//
// Created by cucaracha on 27.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_SHOPMANAGER_H
#define MININGSIMULATORGAMESERVER_SHOPMANAGER_H

#include "PreDefine.h"
#include "Singleton.h"

namespace MS {
    namespace Server {

        class CUser;

        class CShopManager {
        public:
        DECLARE_SINGLETON(CShopManager)

            INT32 Buy(CUser *user, INT32 item_id);
        };


    }
}

#endif //MININGSIMULATORGAMESERVER_SHOPMANAGER_H
