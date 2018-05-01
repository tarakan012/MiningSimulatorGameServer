#include "ServerMS/UserManager.h"
#include "ServerMS/DataSource.h"

namespace ServerMS {

    DB::CDBConnector *CDataSource::GetDBConnector() {
        return m_pDBConnector;
    }

    void CDataSource::Start() {

        m_pDBConnector = new DB::CDBConnector();
        m_pDBConnector->ConnectDB(0, 0, 0, 0);
    }

}