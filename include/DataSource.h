#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "DBManager.h"

namespace MS {
    namespace Server {


#ifdef __cplusplus
        extern "C" {
#endif

        class CDataSource {
        public:

            void Start();

            CDBConnector *GetDBConnector();

        private:
            CDBConnector *m_pDBConnector;
        };


#ifdef __cplusplus
        }
#endif

    }
}
#endif /* DATASOURCE_H */

