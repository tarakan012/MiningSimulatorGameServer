#ifndef PARSEPROTO_H_INCLUDED
#define PARSEPROTO_H_INCLUDED

#include "google/protobuf/message.h"

#include "boost/shared_ptr.hpp"

template<class T>
bool ParseProtoMsg(const char *pData, int n32DataLenth, T &sMsg) {
    return sMsg.ParseFromArray(pData, n32DataLenth);
}

template<class T>
boost::shared_ptr<T> ParseProtoMsgInThread(const char *pData, int n32DataLenth) {
    boost::shared_ptr<T> msg(new T());
    msg->ParseFromArray(pData, n32DataLenth);
    return msg;
}


#endif // PARSEPROTO_H_INCLUDED
