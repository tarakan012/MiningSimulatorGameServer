#include "UserManager.h"

namespace MS {
    namespace Server {
        void CUserManager::UserCacheDBAsynHandler(CBuffer *pBuffer) {
            CDBConnector *pConnector = GetDBSource(0);
            switch (pBuffer->m_LogLvl) {
                case GSToDB::eUserUpdate_DBCallback : {
                    DBAsynUpdateUserCallback(pBuffer, pConnector);
                }
            }
        }

        void CUserManager::DBAsynUpdateUserCallback(CBuffer *pBuffer, CDBConnector *pConnector) {
            GSToDB::UserUpdate sUserUpdate;
            int n32Result = sUserUpdate.ParseFromArray(pBuffer->GetCurData(), pBuffer->GetDataLength());
            if (!n32Result) return;
            pConnector->ExecQuery(sUserUpdate.sql());
            pConnector->CloseQuery();
        }
    }
}
