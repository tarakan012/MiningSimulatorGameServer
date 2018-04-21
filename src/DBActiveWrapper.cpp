#include "DBActiveWrapper.h"

CDBActiveWrapper::CDBActiveWrapper(Callback fCAllBack, BeginInThreadCallback fStartCallBack)
        : m_pActive(CActive::CreateActive(fCAllBack)), m_pDataSource(new MS::Server::CDataSource()),
          m_LogicStartCallBack(fStartCallBack) {
    m_pDataSource->Start();
    m_pActive->SetThreadStartCallback(std::bind(&CDBActiveWrapper::InitInThread, this));
}

void CDBActiveWrapper::Start() {
    m_pActive->Start();
}

void CDBActiveWrapper::InitInThread() {
    if (m_LogicStartCallBack) {
        m_LogicStartCallBack();
    }
}

bool CDBActiveWrapper::EncodeAndSendToDBThread(google::protobuf::Message &rMsg, int n32MsgId) {
    CBuffer *pBuffer = m_pActive->GetBuffer();
    EncodeProtoMsgToBuffer(rMsg, n32MsgId, pBuffer);
    m_pActive->Send(pBuffer);
    return true;
}

bool CDBActiveWrapper::EncodeProtoMsgToBuffer(google::protobuf::Message &rMsg, int n32MsgId, CBuffer *pBuffer) {
    std::string str = rMsg.SerializeAsString();
    if (pBuffer == NULL) return false;
    pBuffer->Append(str);
    pBuffer->m_LogLvl = n32MsgId;
    return true;
}

MS::Server::CDBConnector *CDBActiveWrapper::GetDBConnector() {
    if (m_pDataSource) {
        return m_pDataSource->GetDBConnector();
    }

    return NULL;
}
