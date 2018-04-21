#include "GCInfo.h"

CGCInfo::CGCInfo() {
    m_n32GCID = 0;
    m_tLastConnMilsec = 0;
}

const int CGCInfo::GetGCID() const {
    return m_n32GCID;
}

void CGCInfo::SetGCID(int id) {
    m_n32GCID = id;
}
