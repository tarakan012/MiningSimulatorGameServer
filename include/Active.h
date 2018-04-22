#ifndef ACTIVE_H
#define ACTIVE_H

#include <queue>
#include <string>
#include <functional>
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"
#include "boost/atomic.hpp"
#include "Buffer.h"
#include "ThreadSafeQueue.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef std::function<void(CBuffer *)> Callback;
typedef std::function<void()> BeginInThreadCallback;

class CActive {
public:
    CActive();

    void Start();

    void Stop();

    CBuffer *GetBuffer();

    void ReleaseBuffer(CBuffer *pBuffer);

    void Send(CBuffer *pBuffer);

    void SetThreadStartCallback(BeginInThreadCallback pBeginInThreadCallback);

    static CActive *CreateActive(Callback fCallBack);

    void Run();

    void Consume();

private:
    std::queue<CBuffer *> m_Queue;
    std::queue<CBuffer *> m_QueueSwap;
    boost::mutex m_IOMutex;
    boost::thread m_Thread;
    boost::condition_variable m_ConditionVar;
    Callback m_Callback;
    BeginInThreadCallback m_BeginInThreadCallback;
    CThreadSafeObjectPool<CBuffer> m_pBufferPool;
};


#ifdef __cplusplus
}
#endif

#endif /* ACTIVE_H */

