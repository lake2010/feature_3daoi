#ifndef INSPECTIONMANAGER_HPP
#define INSPECTIONMANAGER_HPP

#include <QDir>

#include "DB/SqliteDb.hpp"

#include "InspectionData.hpp"
#include "InspectionSetting.hpp"
#include "Component.hpp"
#include "FiducialMark.hpp"

namespace Job
{
    /**
     *  @brief InspectionManager
     *          管理设备检测运行的流程
     *  @author lynn
     *  @version 1.00 2018-01-05 lynn
     *                note:create it
     */
    class InspectionManager
    {
    public:

        //>>>-----------------------------------------------------------------------
        //constructor & destructor

        /**
        *  @brief  构造函数
        *  @param  N/A
        *  @return N/A
        */
        InspectionManager();

        /**
        *  @brief  析构函数
        *  @param  N/A
        *  @return N/A
        */
        virtual ~InspectionManager();

        //>>>-----------------------------------------------------------------------
        // set & get function

        InspectionData& inspectionData(){return this->m_inspectionData;}

        InspectionSetting& inspectionSetting(){return this->m_inspectionSetting;}

        QString& inspectionSettingPath(){return this->m_inspectionSettingPath;}

        //>>>-----------------------------------------------------------------------
        // member function

        /**
        *  @brief init
        *           初始化设备检测参数,如初始化相机长,宽等
        *  @param N/A
        *  @return N/A
        */
        void init();

        /**
        *  @brief  loadInspectionSetting
        *            加载相机设置配置文件,设置设备检测时相机参数
        *  @param  path: 相机配置文件的路径
        *  @return N/A
        */
        void loadInspectionSetting(QString &path);

        /**
        *  @brief  loadLibraryData
        *            加载库数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void loadLibraryData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  loadItemData
        *            加载检测框数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void loadItemData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  loadMeasuredObjData
        *            加载被测对象数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void loadMeasuredObjData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  loadBoardData
        *            加载基板数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void loadBoardData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  loadInspectionData
        *            加载检测数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void loadInspectionData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  createInspectionData
        *            创建检测数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void createInspectionData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  createBoardData
        *            创建基板数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void createBoardData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  createMeasuredObjData
        *            创建被测对象数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void createMeasuredObjData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  createLibraryData
        *            创建库数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void createLibraryData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief  createItemData
        *            创建检测框数据
        *  @param  sqlite: 数据库对象
        *  @return N/A
        */
        void createItemData(SSDK::DB::SqliteDB& sqlite);

        /**
        *  @brief inspect
        *           检测基板中所有检测对象
        *  @param  N/A
        *  @return N/A
        */
        void inspect();

    private:

        //>>>-----------------------------------------------------------------------
        // member variant

        InspectionData m_inspectionData;       //检测程式数据
        InspectionSetting m_inspectionSetting; //设备检测时设置,如(设置相机的配置文件)
        QString m_inspectionSettingPath{""};   //设备检测时设置配置文件的路径

        //<<<-----------------------------------------------------------------------
    };

}//End of namespace Job

#endif // INSPECTIONMANAGER_HPP
