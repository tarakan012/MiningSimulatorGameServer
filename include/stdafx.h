#ifndef STDAFX_H
#define STDAFX_H

#include "GCInfo.h"

#include <functional>
#include <map>

#ifdef __cplusplus
extern "C" {
#endif

    namespace MS {
        namespace Server {



            typedef std::function<int( const char* pData, int n32DataLength,CGCInfo * pGCInfo)> PFGCMsgHandler;
            typedef std::map<int,PFGCMsgHandler> GCMsgHandlerMap;
        }
    }

#ifdef __cplusplus
}
#endif

#endif /* STDAFX_H */

