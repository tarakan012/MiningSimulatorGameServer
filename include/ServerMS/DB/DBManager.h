#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "ServerMS/PreDefine.h"
#include "stdio.h"
#include "stdlib.h"
#include "libpq-fe.h"
#include <string>
#include <vector>
#include <map>

namespace DB {

    class CDBConnector {
    public:
        CDBConnector();

        bool ExecQuery(const std::string &rData);

        void QueryNext();

        void CloseQuery();

        void GetQueryFieldData(const CHAR *cpszFieldName, INT32 &rn32Data);

        void GetQueryFieldData(const CHAR *cpszFieldName, UINT64 &rn32Data);

        void GetQueryFieldData(const CHAR *cpszFieldName, std::string &rstrData);

        INT32 GetQueryFieldNum();

        INT32 GetQueryFieldRowNum();

        bool ConnectDB(const CHAR *cpszUserName, const CHAR *cpszUserPassword, const CHAR *cpszHostIP,
                       const CHAR *cpszDBName);

        void GetQueryFields();

    private:
        PGconn *m_pPGConn;
        PGresult *m_pPGResult;
        std::map<std::string, std::string> m_FieldsValue;
        std::vector<std::string> m_FiledVec;
        INT32 m_n32ServerPGVersion;
        INT32 m_TotalFields;
        INT32 m_TotalRows;
        INT32 m_CurRowIndx;
    };

}

#endif /* DBMANAGER_H */