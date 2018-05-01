#include "ServerMS/UserManager.h"

namespace ServerMS {

    void CUserManager::UserCacheDBAsynHandler(CBuffer *pBuffer) {
        DB::CDBConnector *pConnector = GetDBSource(0);
        switch (pBuffer->m_LogLvl) {
            case GSToDB::eUserUpdate_DBCallback : {
                DBAsynUpdateUserCallback(pBuffer, pConnector);
            }
        }
    }

    void CUserManager::DBAsynUpdateUserCallback(CBuffer *pBuffer, DB::CDBConnector *pConnector) {
        GSToDB::UserUpdate sUserUpdate;
        INT32 n32Result = sUserUpdate.ParseFromArray(pBuffer->GetCurData(), pBuffer->GetDataLength());
        if (!n32Result) return;
        pConnector->ExecQuery(sUserUpdate.sql());
        pConnector->CloseQuery();
    }

}