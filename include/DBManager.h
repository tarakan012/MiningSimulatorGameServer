#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "stdio.h"
#include "stdlib.h"
#include "libpq-fe.h"
#include <string>
#include <vector>

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif
        class CDBConnector {
        public:
            CDBConnector();

            bool ExecQuery(const std::string &rData);

            void QueryNext();

            void CloseQuery();

            void GetQueryFieldData(const char *cpszFieldName, int &rn32Data);

            void GetQueryFieldData(const char *cpszFieldName, std::string &rstrData);

            int GetQueryFieldNum();

            int GetQueryFieldRowNum();

            void GetQueryFields();

            bool ConnectDB(const char *cpszUserName, const char *cpszUserPassword, const char *cpszHostIP,
                           const char *cpszDBName);

        private:
            PGconn *m_pPGConn;
            PGresult *m_pPGResult;
            std::map<std::string, std::string> m_FieldsValue;
            std::vector<std::string> m_FiledVec;
            int m_n32ServerPGVersion;
            int m_TotalFields;
            int m_TotalRows;
            int m_CurRowIndx;
        };


#ifdef __cplusplus
        }
#endif

    }
}
#endif /* DBMANAGER_H */

