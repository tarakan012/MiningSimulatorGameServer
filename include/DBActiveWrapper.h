#ifndef DBACTIVEWRAPER_H
#define DBACTIVEWRAPER_H

#include "Active.h"
#include <string>
#include "DataSource.h"
#include "Buffer.h"
#include "google/protobuf/message.h"

#ifdef __cplusplus
extern "C" {
#endif

class CDBActiveWrapper {
public:
    void Start();

    void InitInThread();

    MS::Server::CDBConnector *GetDBConnector();

    bool EncodeAndSendToDBThread(google::protobuf::Message &rMsg, int n32MsgId);

    static bool EncodeProtoMsgToBuffer(google::protobuf::Message &rMsg, int n32MsgId, CBuffer *pBuffer);

    CDBActiveWrapper(Callback fCAllBack, BeginInThreadCallback fStartCallBack = nullptr);

private:
    CActive *m_pActive;
    BeginInThreadCallback m_LogicStartCallBack;
    MS::Server::CDataSource *m_pDataSource;
};


#ifdef __cplusplus
}
#endif

#endif /* DBACTIVEWRAPER_H */

