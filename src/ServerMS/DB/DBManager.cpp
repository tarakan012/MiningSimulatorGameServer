#include "ServerMS/DB/DBManager.h"
#include "ServerMS/PreDefine.h"
#include "ServerMS/Log/Logging.h"
#include <map>

using namespace Log;

namespace DB {

    CDBConnector::CDBConnector()
            : m_pPGConn(NULL),
              m_pPGResult(NULL) {
        m_CurRowIndx = 0;
        m_n32ServerPGVersion = 0;
        m_TotalFields = 0;
        m_TotalRows = 0;
    }

    bool CDBConnector::ExecQuery(const std::string &rData) {
        LogPrintDebug("ExecQuery: %s", rData);
        m_pPGResult = PQexec(m_pPGConn, rData.c_str());
        INT32 status = PQresultStatus(m_pPGResult);
        if (PQresultStatus(m_pPGResult) == PGRES_COMMAND_OK) {

            LogPrintDebug("Query Success. Status: %d", status);
            return true;
        }
        if (PQresultStatus(m_pPGResult) != PGRES_TUPLES_OK) {
            LogPrintError("Query Fail. Status: %d", status);
            return false;
        }

        m_TotalFields = PQnfields(m_pPGResult);
        m_TotalRows = PQntuples(m_pPGResult);
        GetQueryFields();
    }

    void CDBConnector::GetQueryFieldData(const CHAR *cpszFieldName, INT32 &rn32Data) {
        auto iter = m_FieldsValue.find(cpszFieldName);
        if (iter != m_FieldsValue.end()) {
            rn32Data = atoi(iter->second.data());
        }
    }

    void CDBConnector::GetQueryFieldData(const CHAR *cpszFieldName, UINT64 &rn32Data) {
        auto iter = m_FieldsValue.find(cpszFieldName);
        if (iter != m_FieldsValue.end()) {
            rn32Data = atol(iter->second.data());
        }
    }

    void CDBConnector::GetQueryFieldData(const CHAR *cpszFieldName, std::string &rstrData) {
        auto iter = m_FieldsValue.find(cpszFieldName);
        if (iter != m_FieldsValue.end()) {
            rstrData = iter->second.data();
        }
    }

    INT32 CDBConnector::GetQueryFieldRowNum() {
        return m_TotalRows;
    }

    void CDBConnector::GetQueryFields() {
        m_FiledVec.clear();
        std::string strValue;
        std::string strNameColumn;
        m_FieldsValue.clear();
        if (!m_TotalRows) {
            LogPrintDebug("Rows NULL");
            return;
        }
        for (INT32 field = 0; field < m_TotalFields; ++field) {
            strNameColumn = PQfname(m_pPGResult, field);
            strValue = PQgetvalue(m_pPGResult, m_CurRowIndx, field);
            m_FieldsValue[strNameColumn] = strValue;
            m_FiledVec.push_back(strNameColumn);
        }
        m_CurRowIndx++;
    }

    void CDBConnector::CloseQuery() {
        m_FieldsValue.clear();
        m_CurRowIndx = 0;
    }

    void CDBConnector::QueryNext() {
        m_FieldsValue.clear();
        if (m_CurRowIndx == m_TotalRows) {
            return;
        }

        INT32 size = m_FiledVec.size();
        for (INT32 field = 0; field < size; ++field) {
            auto tempRes = PQgetvalue(m_pPGResult, m_CurRowIndx, field);
            m_FieldsValue[m_FiledVec[field]] = tempRes;
        }
        m_CurRowIndx++;
    }

    bool CDBConnector::ConnectDB(const CHAR *cpszUserName, const CHAR *cpszUserPassword, const CHAR *cpszHostIP,
                                 const CHAR *cpszDBName) {
        m_pPGConn = PQconnectdb("user=postgres password=12345 host=127.0.0.1 dbname=gamems");
        if (PQstatus(m_pPGConn) != CONNECTION_OK) {
            LogPrintError("Connect DB Fail");
            return false;
        }
        LogPrint(LogFlags::DB, "Connect DB Success\n");
        m_n32ServerPGVersion = PQserverVersion(m_pPGConn);
        return true;
    }
}