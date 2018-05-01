#ifndef DBACTIVEWRAPER_H
#define DBACTIVEWRAPER_H

#include "ServerMS/PreDefine.h"
#include "ServerMS/Active.h"
#include <string>
#include "ServerMS/DataSource.h"
#include "ServerMS/Buffer.h"
#include "google/protobuf/message.h"

class CDBActiveWrapper {
public:
    void Start();

    void InitInThread();

    DB::CDBConnector *GetDBConnector();

    bool EncodeAndSendToDBThread(google::protobuf::MessageLite &rMsg, INT32 n32MsgId);

    static bool EncodeProtoMsgToBuffer(google::protobuf::MessageLite &rMsg, INT32 n32MsgId, CBuffer *pBuffer);

    CDBActiveWrapper(Callback fCAllBack, BeginInThreadCallback fStartCallBack = nullptr);

private:
    CActive *m_pActive;
    BeginInThreadCallback m_LogicStartCallBack;
    ServerMS::CDataSource *m_pDataSource;
};

#endif /* DBACTIVEWRAPER_H */