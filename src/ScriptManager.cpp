#include "ScriptManager.h"

IMPLEMENT_SINGLETON(CScriptManager);

CScriptManager::CScriptManager() {

}


bool CScriptManager::Initialize() {
    //m_LuaState.open_libraries(sol::lib::base,sol::lib::math);
    LoadScriptFile();
}

bool CScriptManager::LoadScriptFile() {
    //m_LuaState.script_file( "forge.lua" );
}



