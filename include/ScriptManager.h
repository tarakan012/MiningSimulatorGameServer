#ifndef SCRIPT_H
#define SCRIPT_H

#include "Singleton.h"
//#include "sol.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    class CScriptManager
    {
    public:
        CScriptManager();
        DECLARE_SINGLETON(CScriptManager);
        bool LoadScriptFile();
        bool Initialize();
    private:
        //sol::state m_LuaState;
    };


#ifdef __cplusplus
}
#endif

#endif /* SCRIPT_H */

