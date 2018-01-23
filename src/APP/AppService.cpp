#include "AppService.hpp"

using namespace APP;

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// constructor & destructor

AppService::AppService()
{
    try
    {

    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Constructor error!");
}

AppService::~AppService()
{
    try
    {

    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Destructor error!");
}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// member function

void AppService::init()
{
    try
    {
        // 加载配置路径的配置文件
        this->m_pathSetting.load(this->m_pathSettingPath);
        // 加载配置UI设置的配置文件
        this->m_appSetting.load(this->m_pathSetting.appSettingPath());
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Init error!");

}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
