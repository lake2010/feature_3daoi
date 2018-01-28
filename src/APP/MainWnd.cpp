#include "MainWnd.hpp"

using namespace std;

using namespace APP;
using namespace Job;
using namespace SSDK;
using namespace SSDK::DB;

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// constructor & destructor

MainWnd::MainWnd()
{
    try
    {

    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Constructor error!");
}

MainWnd::~MainWnd()
{
    try
    {

    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Destructor error!");
}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// member function

void MainWnd::run()
{
    try
    {
        // 扫描检测程式
        this->scanJobFolder();

        // 加载完程式,检测
        cout << "Press \"y\" to inspect:";
        char inspectValue;
        cin >> inspectValue;
        while ( 'y' == inspectValue || 'Y' == inspectValue )
        {
            this->inspectClick();
            this->writeNGToXml();
            cout << "Press \"y\" to inspect again:";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> inspectValue;
        }
        cout << "Quit ..." << endl;
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Running main window error!");
}

void MainWnd::scanJobFolder()
{
    QString path = APP::g_pAppService->pathSetting().defaultJobFolderPath();
    try
    {
        //>>>-------------------------------------------------------------------
        // 1.判断文件夹是否存在
        QDir dir(path);
        if(!dir.exists())
        {
            std::cout << path.toStdString() << " is not exists!"<< std::endl;
            THROW_EXCEPTION("No job path!");
        }
        //>>>-------------------------------------------------------------------
        // 2.扫描程式文件

        dir.setFilter(QDir::Files);
        QFileInfoList list = dir.entryInfoList();

        // 先显示"创建程式"的提示信息,再将扫描到的文件打印
        cout << "0:Create default job" << endl;
        for (int i = 0; i < list.size(); ++i)
        {
            cout<<qPrintable(QString("%1:%2\n").arg(i+1).arg(list.at(i).fileName()));
        }
        int index {-1};
        cout << "\nChoice 0 to create job other to load job:";
        // 创建程式或选择需要加载的程式文件
        while( index > list.size() || index < 0)
        {
            cin >> index;
            if(!cin)            // 输入不是数字
            {
                cout<<"Not a digit. Please input 0 to "<<list.size()<<":";
                index = -1;
            }
            else if( index > list.size() || index < 0 ) // 输入不在范围内
            {
                cout<<"Out of range! Please input 0 to "<<list.size() <<":";
            }
            cin.clear();        // 清空键盘输入的缓存
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        //>>>-------------------------------------------------------------------
        // 3.创建或加载程式

        if( 0 == index )        // 创建新的程式
        {
            DataGenerator dataGenerator;
            dataGenerator.generateInspectionData(g_pSequence->inspectionManager().inspectionData());

            QString jobPath {path};
            string jobName;
            cout << "Please input job name:";
            getline(cin,jobName);
            jobPath.append(QString::fromStdString(jobName));
            createDefaultJob(jobPath);
        }
        else                    // 加载用户选择的程式
        {
            QString jobPath = list.at(index-1).filePath();
            loadJob(jobPath);
        }
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Catch exception and rethrow");
}

void MainWnd::inspectClick()
{
    try
    {
        APP::g_pSequence->inspectCycle();
    }
    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ("Inspect click error!");
}

void MainWnd::writeNGToXml()
{
    string xmlFileName{""};     // xml文件名
    QString xmlPath{""};        // xml文件路径

    xmlFileName = APP::g_pSequence->inspectionManager().inspectionData().board()
            .name() + "_" + DataGenerator::getCurrentTime()+".xml";
    xmlPath = APP::g_pAppService->pathSetting().exportXmlPath() +
            xmlFileName.c_str();

    // 将NG的元件写入到xml文件中
    APP::g_pSequence->inspectionManager().inspectionData().
            writeInspectionDataToXml( xmlPath );
}

void MainWnd::loadJob(const QString& jobPath)
{
    SqliteDB sqlite( jobPath.toStdString() );
    try
    {
        // 打开数据库
        auto isOpened = sqlite.isOpened();
        if( isOpened )
        {
            g_pSequence->inspectionManager().loadLibraryData(sqlite);
            g_pSequence->inspectionManager().loadItemData(sqlite);
            g_pSequence->inspectionManager().loadMeasuredObjData(sqlite);
            g_pSequence->inspectionManager().loadBoardData(sqlite);
            g_pSequence->inspectionManager().loadInspectionData(sqlite);

            sqlite.reset();
            sqlite.close();
        }
        else
        {
            THROW_EXCEPTION("程式加载失败！");
        }
    }
    catch( const CustomException& ex )
    {
        if( sqlite.isOpened() )
        {
            sqlite.reset();
            sqlite.close();
        }
        THROW_EXCEPTION( ex.what() );
    }
}

void MainWnd::createDefaultJob(const QString& jobName)
{
    SqliteDB sqlite;
    try
    {
        // 创建数据库对象，打开传入路径的数据库
        sqlite.open( jobName.toStdString() );

        if( !sqlite.isOpened() )
        {
            THROW_EXCEPTION("数据库打开失败！");
        }
        g_pSequence->inspectionManager().createInspectionData(sqlite);
        g_pSequence->inspectionManager().createBoardData(sqlite);
        g_pSequence->inspectionManager().createMeasuredObjData(sqlite);
        g_pSequence->inspectionManager().createLibraryData(sqlite);
        g_pSequence->inspectionManager().createItemData(sqlite);

        sqlite.close();
    }
    catch( const CustomException& ex )
    {
        if( sqlite.isOpened() )
        {
            sqlite.reset();
            sqlite.close();
        }
        THROW_EXCEPTION( ex.what() );
    }
}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
