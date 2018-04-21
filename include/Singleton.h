#ifndef SINGLETON_H
#define SINGLETON_H


#ifdef __cplusplus
extern "C" {
#endif

#define DECLARE_SINGLETON(cls) \
private: \
static cls * m_pInstance; \
public: \
static cls & GetInstance() \
{ \
    if (nullptr == m_pInstance){ \
    m_pInstance = new cls(); \
    } \
    return *m_pInstance; \
} \

#define IMPLEMENT_SINGLETON(cls) \
    cls* cls::m_pInstance = nullptr;

#ifdef __cplusplus
}
#endif

#endif /* SINGLETON_H */

