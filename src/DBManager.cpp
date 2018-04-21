#include <map>
#include "util.h"
#include "DBManager.h"

namespace MS {
    namespace Server {

        CDBConnector::CDBConnector()
                : m_pPGConn(NULL),
                  m_pPGResult(NULL) {
            m_CurRowIndx = 0;
            m_n32ServerPGVersion = 0;
            m_TotalFields = 0;
            m_TotalRows = 0;
        }

        bool CDBConnector::ExecQuery(const std::string &rData) {
            LogPrint(LogFlags::ALL, "ExecQuery: %s\n", rData);
            m_pPGResult = PQexec(m_pPGConn, rData.c_str());
            int status = PQresultStatus(m_pPGResult);
            if (PQresultStatus(m_pPGResult) == PGRES_COMMAND_OK) {

                LogPrint(LogFlags::DB, "Query Success. Status: %d\n", status);
                return true;
            }
            if (PQresultStatus(m_pPGResult) != PGRES_TUPLES_OK) {
                LogPrint(LogFlags::DB, "Query Fail. Status: %d\n", status);
                return false;
            }

            m_TotalFields = PQnfields(m_pPGResult);
            m_TotalRows = PQntuples(m_pPGResult);
            GetQueryFields();
        }

        void CDBConnector::GetQueryFieldData(const char *cpszFieldName, int &rn32Data) {
            auto iter = m_FieldsValue.find(cpszFieldName);
            if (iter != m_FieldsValue.end()) {
                rn32Data = atoi(iter->second.data());
            }
        }

        void CDBConnector::GetQueryFieldData(const char *cpszFieldName, std::string &rstrData) {
            auto iter = m_FieldsValue.find(cpszFieldName);
            if (iter != m_FieldsValue.end()) {
                rstrData = iter->second.data();
            }
        }

        int CDBConnector::GetQueryFieldRowNum() {
            return m_TotalRows;
        }

        void CDBConnector::GetQueryFields() {
            m_FiledVec.clear();
            std::string strValue;
            std::string strNameColumn;
            m_FieldsValue.clear();
            if (!m_TotalRows) {
                LogPrint(LogFlags::DB, "Rows NULL\n");
                return;
            }
            for (int field = 0; field < m_TotalFields; ++field) {
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

            int size = m_FiledVec.size();
            for (int field = 0; field < size; ++field) {
                auto tempRes = PQgetvalue(m_pPGResult, m_CurRowIndx, field);
                m_FieldsValue[m_FiledVec[field]] = tempRes;
            }
            m_CurRowIndx++;
        }

        bool CDBConnector::ConnectDB(const char *cpszUserName, const char *cpszUserPassword, const char *cpszHostIP,
                                     const char *cpszDBName) {
            m_pPGConn = PQconnectdb("user=postgres password=12345 host=127.0.0.1 dbname=gamems");
            if (PQstatus(m_pPGConn) != CONNECTION_OK) {
                LogPrint(LogFlags::DB, "Connect DB Fail\n");
                return false;
            }
            LogPrint(LogFlags::DB, "Connect DB Success\n");
            m_n32ServerPGVersion = PQserverVersion(m_pPGConn);
            return true;
        }
    }
}