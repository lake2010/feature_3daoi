#ifndef PATHSETTING_HPP
#define PATHSETTING_HPP

#include <iostream>

#include <QDir>
#include <QFile>
#include <QSettings>

#include "CustomException.hpp"

namespace APP
{
    /**
     *  @brief PathSetting
     *          PathSetting类是存放各个配置文件路径的类
     *             例如:软件界面配置文件的路径、检测程式文件的路径等
     *  @author peter
     *  @version 1.00 2018-01-07 peter
     *                note:create it
     */
    class PathSetting
    {
    public:

        //>>>-------------------------------------------------------------------
        // constructor & destructor

        PathSetting();

        virtual ~PathSetting();

        //>>>-------------------------------------------------------------------
        // set & get function

        QString appSettingPath(){return this->m_appSettingPath;}

        QString xmlPath(){return this->m_xmlPath;}

        QString defaultJobFolderPath(){return this->m_defaultJobFolderPath;}

        QString inspectionSettingPath(){return this->m_inspectionSettingPath;}

        QString userAccountPath(){return this->m_userAccountPath;}

        //>>>-------------------------------------------------------------------
        // member function

        /**
         *  @brief load
         *          该函数功能是读取配置文件中的信息
         *  @param path
         *          待读取的配置文件路径
         *  @return N/A
         */
        void load(const QString& path);


    private:

        //>>>-------------------------------------------------------------------
        // member variant

        QString m_appSettingPath {""};          // 软件界面的配置文件路径
        QString m_defaultJobFolderPath {""};    // 默认检测程式的路径
        QString m_inspectionSettingPath {""};   // 设备检测信息的配置文件路径
        QString m_xmlPath {""};                 // 导出到xml的路径
        QString m_userAccountPath {""};         // 用户注册信息的配置文件路径

        //<<<-------------------------------------------------------------------

    };
}// End of namespace APP

#endif // PATHSETTING_HPP
