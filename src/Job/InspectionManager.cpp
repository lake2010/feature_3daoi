#include "InspectionManager.hpp"

using namespace Job;
using namespace SSDK;
using namespace SSDK::DB;

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// constructor & destructor

InspectionManager::InspectionManager()
{

}

InspectionManager::~InspectionManager()
{

}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// member function

void InspectionManager::init()
{
    try
    {
        loadInspectionSetting(this->m_inspectionSettingPath);
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("InspectionManager init error!");
}

void InspectionManager::loadInspectionSetting(QString &path)
{
    try
    {
        this->m_inspectionSetting.load(path);
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Load inspectionSetting file error");
}

void InspectionManager::loadLibraryData(SqliteDB& sqlite)
{
    // 读取Library表
    string selectedString = "select * from Library";
    sqlite.prepare( selectedString );
    sqlite.begin();

    // 读取数据表中信息并存到Library容器中
    Library* pLibrary = nullptr;
    while(true)
    {
        sqlite.step();
        if(sqlite.latestErrorCode() == SQLITE_DONE)
        {
            break;
        }
        pLibrary = new Library();
        pLibrary->setId( boost::get<int>(sqlite.columnValue(0)) );
        pLibrary->name() = boost::get<string>(sqlite.columnValue(1));
        this->m_inspectionData.board().pLibs().push_back(pLibrary);
    }
}

void InspectionManager::loadItemData(SqliteDB& sqlite)
{
    //>>>-------------------------------------------------------------------------------------------------------------------------------------
    //1 读取MainItem表
    string selectedString = "select * from MainItem";
    sqlite.prepare( selectedString );
    sqlite.begin();

    // 使用迭代器访问容器vector中的元素
    vector<Library *>::iterator it = this->m_inspectionData.board().pLibs().begin();
    while(true)
    {
        sqlite.step();
        if(sqlite.latestErrorCode() == SQLITE_DONE)
        {
            break;
        }
        // 读取并判断数据表中的算法类型，然后在内存中设置相应的算法类型
        if ( VAR_TO_STR(Alg::AlgType::ALG3D) == boost::get<string>(sqlite.columnValue(8)) )
        {
            (*it)->mainItem().setPAlg( new Alg3D() );
        }
        else if ( VAR_TO_STR(Alg::AlgType::ALG2D) == boost::get<string>(sqlite.columnValue(8)) )
        {
            (*it)->mainItem().setPAlg( new Alg2D() );
        }
        else
        {
            THROW_EXCEPTION("MainItem算法类型读取失败！");
        }
        (*it)->mainItem().setLibId( boost::get<int>(sqlite.columnValue(0)) );
        (*it)->mainItem().setId( boost::get<int>(sqlite.columnValue(1)) );
        (*it)->mainItem().rectangle().centerPoint().setPosX( boost::get<double>(sqlite.columnValue(2)) );
        (*it)->mainItem().rectangle().centerPoint().setPosY( boost::get<double>(sqlite.columnValue(3)) );
        (*it)->mainItem().rectangle().setWidth( boost::get<double>(sqlite.columnValue(4)) );
        (*it)->mainItem().rectangle().setHeight( boost::get<double>(sqlite.columnValue(5)) );
        (*it)->mainItem().rectangle().setAngle( boost::get<double>(sqlite.columnValue(6)) );
        (*it)->mainItem().name() = boost::get<string>(sqlite.columnValue(7));
        ++it;
    }

    //>>>-------------------------------------------------------------------------------------------------------------------------------------
    //2 读取SubItem表
    selectedString = "select * from SubItem";
    sqlite.prepare( selectedString );
    sqlite.begin();

    while(true)
    {
        sqlite.step();
        if(sqlite.latestErrorCode() == SQLITE_DONE)
        {
            break;
        }
        for (vector<Library *>::iterator i = this->m_inspectionData.board().pLibs().begin();
             i != this->m_inspectionData.board().pLibs().end(); ++i)
        {
            // 在Library容器中找到与数据表中相应的库的ID号，然后将数据表中的信息存放到SubItem容器中
            if ( (*i)->id() == boost::get<int>(sqlite.columnValue(0)) )
            {
                (*i)->subItems().push_back(SubItem());
                SubItem& item = (*i)->subItems().back();
                if ( VAR_TO_STR(Alg::AlgType::ALG2D) == boost::get<string>(sqlite.columnValue(8)) )
                {
                    item.setPAlg( new Alg2D() );
                }
                else if ( VAR_TO_STR(Alg::AlgType::ALG3D) == boost::get<string>(sqlite.columnValue(8)) )
                {
                    item.setPAlg( new Alg3D() );
                }
                else
                {
                    THROW_EXCEPTION("SubItem算法类型读取失败！");
                }
                item.setLibId( boost::get<int>(sqlite.columnValue(0)) );
                item.setId( boost::get<int>(sqlite.columnValue(1)) );
                item.rectangle().centerPoint().setPosX( boost::get<double>(sqlite.columnValue(2)) );
                item.rectangle().centerPoint().setPosY( boost::get<double>(sqlite.columnValue(3)) );
                item.rectangle().setWidth( boost::get<double>(sqlite.columnValue(4)) );
                item.rectangle().setHeight( boost::get<double>(sqlite.columnValue(5)) );
                item.rectangle().setAngle( boost::get<double>(sqlite.columnValue(6)) );
                item.name() = boost::get<string>(sqlite.columnValue(7));
                item.setOrder( boost::get<int>(sqlite.columnValue(9)) );
                break;
            }
        }
    }
}

void InspectionManager::loadMeasuredObjData(SqliteDB& sqlite)
{
    // 读取MeasuredObjs表
    string selectedString = "select * from MeasuredObjs";
    sqlite.prepare( selectedString );
    sqlite.begin();

    MeasuredObj *pMeasuredObj = nullptr;
    while(true)
    {
        sqlite.step();
        if(sqlite.latestErrorCode() == SQLITE_DONE)
        {
            break;
        }
        // 读取并判断数据表中的被测对象类型，然后创建相应的类型的对象
        if (VAR_TO_STR(MeasuredObj::MeasuredObjType::FIDUCIALMARK) == boost::get<string>(sqlite.columnValue(8)) )
        {
            pMeasuredObj = new FiducialMark();
        }
        else if (VAR_TO_STR(MeasuredObj::MeasuredObjType::COMPONENT) == boost::get<string>(sqlite.columnValue(8)) )
        {
            pMeasuredObj = new Component();
        }
        else
        {
            THROW_EXCEPTION("元件类型读取失败！");
        }
        pMeasuredObj->setId( boost::get<int>(sqlite.columnValue(0)) );
        pMeasuredObj->name() = boost::get<string>(sqlite.columnValue(1));
        pMeasuredObj->rectangle().centerPoint().setPosX( boost::get<double>(sqlite.columnValue(2)) );
        pMeasuredObj->rectangle().centerPoint().setPosY( boost::get<double>(sqlite.columnValue(3)) );
        pMeasuredObj->rectangle().setWidth( boost::get<double>(sqlite.columnValue(4)) );
        pMeasuredObj->rectangle().setHeight( boost::get<double>(sqlite.columnValue(5)) );
        pMeasuredObj->rectangle().setAngle( boost::get<double>(sqlite.columnValue(6)) );

        // 在Library容器中找到与数据表中相应的库的ID号，然后设置相应的库的信息
        for (vector<Library *>::iterator i = this->m_inspectionData.board().pLibs().begin();
             i != this->m_inspectionData.board().pLibs().end(); ++i)
        {
            if( (*i)->id() == boost::get<int>(sqlite.columnValue(7)) )
            {
                pMeasuredObj->setPLib((*i));
                break;
            }
        }
        this->m_inspectionData.board().pObjs().push_back(pMeasuredObj);
    }
}

void InspectionManager::loadBoardData(SqliteDB& sqlite)
{
    // 读取Board表
    string selectedString = "select * from Board";
    sqlite.prepare( selectedString );
    sqlite.begin();

    while( true )
    {
        sqlite.step();
        if ( sqlite.latestErrorCode() == SQLITE_DONE )
        {
            break;
        }
        this->m_inspectionData.board().name() = boost::get<string>(sqlite.columnValue(0));
        this->m_inspectionData.board().rectangle().setWidth( boost::get<double>(sqlite.columnValue(1)) );
        this->m_inspectionData.board().rectangle().setHeight( boost::get<double>(sqlite.columnValue(2)) );
        this->m_inspectionData.board().originPoint().setPosX( boost::get<double>(sqlite.columnValue(3)) );
        this->m_inspectionData.board().originPoint().setPosY( boost::get<double>(sqlite.columnValue(4)) );
    }
}

void InspectionManager::loadInspectionData(SqliteDB& sqlite)
{
    // 读取InspectionData表
    // 查询数据表中所有内容
    string selectedString = "select * from InspectionData";
    sqlite.prepare( selectedString );
    sqlite.begin();

    while( true )
    {
        // 逐行读取
        sqlite.step();
        if ( sqlite.latestErrorCode() == SQLITE_DONE )
        {
            break;
        }
        // 按列读取（第一列为版本号，第二列为上次编辑时间）
        this->m_inspectionData.version() = boost::get<string>(sqlite.columnValue(0));
        this->m_inspectionData.lastEditingTime() = boost::get<string>(sqlite.columnValue(1));
    }
}

void InspectionManager::createInspectionData(SqliteDB& sqlite)
{
    // 创建InspectionData表，表中包含字段：Version、LastEditingTime
    string sqlCreate = "CREATE TABLE InspectionData( "
                       "Version TEXT, "
                       "LastEditingTime TEXT );";
    sqlite.execute( sqlCreate );
    // 插入各字段对应的数据
    string sqlInsert = "INSERT INTO InspectionData( "
                       "Version, LastEditingTime ) "
                       "VALUES(?,?);";
    sqlite.execute( sqlInsert, this->m_inspectionData.version(),
                               this->m_inspectionData.lastEditingTime() );
}

void InspectionManager::createBoardData(SqliteDB& sqlite)
{
    // 创建Board表，表中包含字段：Name、SizeX、SizeY、OriginalX、OriginalY
    string sqlCreate = "CREATE TABLE Board( "
                       "Name TEXT, "
                       "SizeX REAL, "
                       "SizeY REAL, "
                       "OriginalX REAL, "
                       "OriginalY REAL );";
    sqlite.execute( sqlCreate );

    string sqlInsert = "INSERT INTO Board( "
                       "Name, SizeX, SizeY, OriginalX, OriginalY ) "
                       "VALUES(?,?,?,?,?);";
    sqlite.execute( sqlInsert, this->m_inspectionData.board().name(),
                               this->m_inspectionData.board().rectangle().width(),
                               this->m_inspectionData.board().rectangle().height(),
                               this->m_inspectionData.board().originPoint().posX(),
                               this->m_inspectionData.board().originPoint().posY() );

}

void InspectionManager::createMeasuredObjData(SqliteDB& sqlite)
{
    // 创建MeasuredObjs表，表中包含字段：ID、Name、CentralX、CentralY、Width、Height、Angle、LibID、MeasuredObjType
    string sqlCreate = "CREATE TABLE MeasuredObjs( "
                       "ID INTEGER, "
                       "Name TEXT, "
                       "CentralX REAL, "
                       "CentralY REAL, "
                       "Width REAL, "
                       "Height REAL, "
                       "Angle REAL, "
                       "LibID INTEGER, "
                       "MeasuredObjType TEXT );";
    sqlite.execute( sqlCreate );

    string sqlInsert = "INSERT INTO MeasuredObjs( "
                       "ID, Name, CentralX, CentralY, Width, Height, Angle, LibID, MeasuredObjType ) "
                       "VALUES(?,?,?,?,?,?,?,?,?);";

    string measuredObjType = "";

    sqlite.prepare( sqlInsert );
    sqlite.begin();
    for (vector<MeasuredObj *>::iterator i = this->m_inspectionData.board().pObjs().begin();
         i != this->m_inspectionData.board().pObjs().end(); ++i)
    {
        // 判断生成的被测对象类型，然后设置相应的类型
        if (MeasuredObj::MeasuredObjType::FIDUCIALMARK == (*i)->measuredObjType() )
        {
            measuredObjType = VAR_TO_STR(MeasuredObj::MeasuredObjType::FIDUCIALMARK);
        }
        else if (MeasuredObj::MeasuredObjType::COMPONENT == (*i)->measuredObjType() )
        {
            measuredObjType = VAR_TO_STR(MeasuredObj::MeasuredObjType::COMPONENT);
        }
        else
        {
            THROW_EXCEPTION("元件类型错误！");
        }
        sqlite.execute( sqlInsert, (*i)->id(),
                                   (*i)->name(),
                                   (*i)->rectangle().centerPoint().posX(),
                                   (*i)->rectangle().centerPoint().posY(),
                                   (*i)->rectangle().width(),
                                   (*i)->rectangle().height(),
                                   (*i)->rectangle().angle(),
                                   (*i)->pLib()->id(),
                                   measuredObjType );
    }
    sqlite.commit();
}

void InspectionManager::createLibraryData(SqliteDB& sqlite)
{
    // 创建Library表，表中包含字段：ID、Name
    string sqlCreate = "CREATE TABLE Library( "
                       "ID INTEGER, "
                       "Name TEXT );";
    sqlite.execute( sqlCreate );

    string sqlInsert = "INSERT INTO Library( "
                       "ID, Name) "
                       "VALUES(?,?);";

    sqlite.prepare( sqlInsert );
    sqlite.begin();
    for (vector<Library *>::iterator i = this->m_inspectionData.board().pLibs().begin();
         i != this->m_inspectionData.board().pLibs().end(); ++i)
    {
        sqlite.execute( sqlInsert, (*i)->id(),
                                   (*i)->name() );
    }
    sqlite.commit();
}

void InspectionManager::createItemData(SqliteDB& sqlite)
{
    //>>>-------------------------------------------------------------------------------------------------------------------------------------
    //1 创建MainItem表，表中包含字段：LibID、ID、ShiftX、ShiftY、Width、Height、Angle、Name、Alg
    string sqlCreate = "CREATE TABLE MainItem( "
                       "LibID INTEGER, "
                       "ID INTEGER, "
                       "ShiftX REAL, "
                       "ShiftY REAL, "
                       "Width REAL, "
                       "Height REAL, "
                       "Angle REAL, "
                       "Name TEXT, "
                       "Alg TEXT );";
    sqlite.execute( sqlCreate );

    string sqlInsert = "INSERT INTO MainItem( "
                       "LibID, ID, ShiftX, ShiftY, Width, Height, Angle, Name, Alg) "
                       "VALUES(?,?,?,?,?,?,?,?,?);";
    string algType = "";

    sqlite.prepare( sqlInsert );
    sqlite.begin();
    for (vector<Library *>::iterator i = this->m_inspectionData.board().pLibs().begin();
         i != this->m_inspectionData.board().pLibs().end(); ++i)
    {
        // 判断生成的算法类型，然后设置相应的类型
        if (Alg::AlgType::ALG3D == (*i)->mainItem().pAlg()->algType())
        {
            algType = VAR_TO_STR(Alg::AlgType::ALG3D);
        }
        else if (Alg::AlgType::ALG2D == (*i)->mainItem().pAlg()->algType() )
        {
            algType = VAR_TO_STR(Alg::AlgType::ALG2D);
        }
        else
        {
            THROW_EXCEPTION("MainItem算法类型错误！");
        }
        sqlite.execute( sqlInsert, (*i)->mainItem().libId(),
                                   (*i)->mainItem().id(),
                                   (*i)->mainItem().rectangle().centerPoint().posX(),
                                   (*i)->mainItem().rectangle().centerPoint().posY(),
                                   (*i)->mainItem().rectangle().width(),
                                   (*i)->mainItem().rectangle().height(),
                                   (*i)->mainItem().rectangle().angle(),
                                   (*i)->mainItem().name(),
                                   algType );
    }
    sqlite.commit();

    //>>>-------------------------------------------------------------------------------------------------------------------------------------
    //2 创建SubItem表，表中包含字段：LibID、ID、ShiftX、ShiftY、Width、Height、Angle、Name、Alg、Order
    sqlCreate = "CREATE TABLE SubItem( "
                "LibID INTEGER, "
                "ID INTEGER, "
                "ShiftX REAL, "
                "ShiftY REAL, "
                "Width REAL, "
                "Height REAL, "
                "Angle REAL, "
                "Name TEXT, "
                "Alg TEXT, "
                "'Order' INTEGER );";
    sqlite.execute( sqlCreate );

    sqlInsert = "INSERT INTO SubItem( "
                "LibID, ID, ShiftX, ShiftY, Width, Height, Angle, Name, Alg, 'Order') "
                "VALUES(?,?,?,?,?,?,?,?,?,?);";

    sqlite.prepare( sqlInsert );
    sqlite.begin();
    for (size_t i = 0; i < this->m_inspectionData.board().pLibs().size(); ++i)
    {
        for(list<SubItem>::iterator j = this->m_inspectionData.board().pLibs()[i]->subItems().begin();
            j != this->m_inspectionData.board().pLibs()[i]->subItems().end(); ++j)
        {
            if (Alg::AlgType::ALG2D == (*j).pAlg()->algType())
            {
                algType = VAR_TO_STR(Alg::AlgType::ALG2D);
            }
            else if (Alg::AlgType::ALG3D == (*j).pAlg()->algType() )
            {
                algType = VAR_TO_STR(Alg::AlgType::ALG2D);
            }
            else
            {
                THROW_EXCEPTION("SubItem算法类型错误！");
            }
            sqlite.execute( sqlInsert, (*j).libId(),
                                       (*j).id(),
                                       (*j).rectangle().centerPoint().posX(),
                                       (*j).rectangle().centerPoint().posY(),
                                       (*j).rectangle().width(),
                                       (*j).rectangle().height(),
                                       (*j).rectangle().angle(),
                                       (*j).name(),
                                       algType,
                                       (*j).order() );
        }
    }
    sqlite.commit();
}

void InspectionManager::inspect()
{
    try
    {
        this->m_inspectionData.inspect();
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("InspectionManager inspect error!");
}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
