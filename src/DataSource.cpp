#include "UserManager.h"
#include "DataSource.h"

namespace MS {
    namespace Server {

        CDBConnector *CDataSource::GetDBConnector() {
            return m_pDBConnector;
        }

        void CDataSource::Start() {

            m_pDBConnector = new CDBConnector();
            m_pDBConnector->ConnectDB(0, 0, 0, 0);
        }


    }
}
