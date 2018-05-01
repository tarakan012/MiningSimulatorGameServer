#ifndef STDAFX_H
#define STDAFX_H

#include "stdio.h"
#include <string>
#include <vector>
#include <functional>
#include <queue>
#include <map>
#include <boost/shared_ptr.hpp>
#include "boost/atomic.hpp"
#include "google/protobuf/message.h"

#include "ServerMS/PreDefine.h"
#include "ServerMS/UserNetInfo.h"

namespace ServerMS {

    typedef std::function<int(const char *pData, int n32DataLength, SUserNetInfo netinfo)> PFGCMsgHandler;
    typedef std::map<int, PFGCMsgHandler> GCMsgHandlerMap;

}

#endif /* STDAFX_H */