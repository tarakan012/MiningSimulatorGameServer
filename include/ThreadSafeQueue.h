#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include "PreDefine.h"
#include <boost/atomic.hpp>
#include <queue>

template<class T>
class CThreadSafeObjectPool {
public:
    CThreadSafeObjectPool(INT32 blockSize = 1024, INT32 n32BufferSize = 1024) : m_DefaultBlockSize(blockSize),
                                                                            m_BufferSize(n32BufferSize),
                                                                            m_TotBufferSize(0) {}

    ~CThreadSafeObjectPool() {
        while (m_ObjectBlock.empty()) {
            T *pBuffer = NULL;
            pBuffer = m_ObjectBlock.back();
            if (NULL != pBuffer) {
                delete pBuffer;
            }
        }
    }

    T *AcquireObject() {
        if (true == m_ObjectBlock.empty()) {
            for (INT32 i = 0; i < m_DefaultBlockSize; ++i) {
                T *pNewObject = new T(m_BufferSize);
                m_ObjectBlock.push(pNewObject);
                ++m_TotBufferSize;
            }
        }

        T *pGetObject = NULL;
        pGetObject = m_ObjectBlock.front();
        m_ObjectBlock.pop();
        if (!pGetObject) {
            pGetObject = new T(m_BufferSize);
            ++m_TotBufferSize;
        }
        return pGetObject;
    }

    void ReleaseObejct(T *pObject) {
        if (nullptr == pObject) return;
        if (m_TotBufferSize > m_DefaultBlockSize * 2) {
            --m_TotBufferSize;
            delete pObject;
            return;
        }
        pObject->Clear();
        m_ObjectBlock.push(pObject);
    }

private:
    const INT32 m_DefaultBlockSize;
    const INT32 m_BufferSize;
    boost::atomic_int32_t m_TotBufferSize;
    std::queue<T *> m_ObjectBlock;

};


#endif /* THREADSAFEQUEUE_H */

