#include "Active.h"

CActive::CActive()
        : m_BeginInThreadCallback(NULL) {

}

void CActive::Run() {
    if (m_BeginInThreadCallback) {
        m_BeginInThreadCallback();
    }
    while (true) {

        Consume();
    }

}

void CActive::SetThreadStartCallback(BeginInThreadCallback pBeginInThreadCallback) {
    m_BeginInThreadCallback = pBeginInThreadCallback;
}


void CActive::Start() {
    m_Thread = boost::thread(&CActive::Run, this);
}

CBuffer *CActive::GetBuffer() {
    CBuffer *pBuffer = m_pBufferPool.AcquireObject();
    if (!pBuffer) return NULL;
    pBuffer->Clear();
    return pBuffer;
}

void CActive::ReleaseBuffer(CBuffer *pBuffer) {
    if (NULL != pBuffer) {
        m_pBufferPool.ReleaseObejct(pBuffer);
        pBuffer = NULL;
    }

}

void CActive::Stop() {
    m_ConditionVar.notify_one();
}

CActive *CActive::CreateActive(Callback fCallBack) {
    CActive *pAct = new CActive();
    pAct->m_Callback = fCallBack;
    return pAct;
}

void CActive::Send(CBuffer *pBuffer) {
    boost::mutex::scoped_lock lock(m_IOMutex);
    m_Queue.push(pBuffer);
    lock.unlock();
    m_ConditionVar.notify_one();
}


void CActive::Consume() {
    boost::mutex::scoped_lock lock(m_IOMutex);
    while (m_Queue.empty()) {
        m_ConditionVar.wait(lock);
    }
    m_QueueSwap.swap(m_Queue);
    while (!m_QueueSwap.empty()) {
        CBuffer *pBuffer = m_QueueSwap.front();
        m_QueueSwap.pop();
        m_Callback(pBuffer);
    }


}
