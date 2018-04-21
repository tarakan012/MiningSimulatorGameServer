#ifndef GCINFO_INCLUDED
#define GCINFO_INCLUDED

#include "PreDefine.h"

class CGCInfo
{
public:
    CGCInfo();
    const int GetGCID() const;
    void SetGCID(int id);
private:
    int m_n32GCID;
    TIME_MILSEC m_tLastConnMilsec;

};

#endif // GCINFO_INCLUDED
