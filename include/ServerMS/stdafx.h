#ifndef STDAFX_H
#define STDAFX_H

#include "ServerMS/UserNetInfo.h"

#include <functional>
#include <map>

namespace ServerMS {

    typedef std::function<int(const char *pData, int n32DataLength, SUserNetInfo netinfo)> PFGCMsgHandler;
    typedef std::map<int, PFGCMsgHandler> GCMsgHandlerMap;

}

#endif /* STDAFX_H */