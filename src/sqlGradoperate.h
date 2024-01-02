
#ifndef SQLOPERATE_H
#define SQLOPERATE_H

#include"cmdresult.h"
#include <utility>
#include<string>
#include"SimpleIni.h"
extern "C"
{
#include"DMsql.h"
}

#define SYSTEM_INI_FILE_PATH "/root/CenterExceptional/config/system_info.ini"
#define PATH_DATABASE_CONFIG    "/root/CenterExceptional/config/db_config.ini" //数据库配置文件的地址

class CSqlGradOperate 
{
public:
    explicit CSqlGradOperate();
    ~CSqlGradOperate();

    
public:
    //初始化数据库的
    int InitDMShemaAndTable();
    //将数据存入数据库中
    int DataInsert(const char *pcQuery);



    //根据和表名字行号查询数据库，获取数据库的数据信息
    int GetTableDataByNameLimitRows(const char* ptableName, unsigned long iCurrentPageNumber, PersonnalDataStruct** jsonOUt);
     //获取数据库数据对象
    int GetTableList(struct DataBase_Table_list** tablelist);
    //根据Id和表名查找数据
    int GetTableDataByIdAndTableName(const char* ptableName, unsigned long iCurrentPageNumber, const std::string& key, const std::string& value , PersonnalDataStruct** jsonOUt);
    //获取数据的流转路径
    int GetDataPath(const char* ptableName, const std::string& key, const std::string& value , PersonnalDataStruct** jsonOUt);
    //根据ke,表名字来查询指定的字段
    int GetDataSearchByKey(const char* ptableName,const dataForSearch* dataSearch, const std::string& key, const std::string& value , PersonnalDataStruct** jsonOUt);

private:
    //获取节内的配置项的值(int)
    int GetItemInt(struct St_NSP_PDCSMCS_SectionIni *pstItem,  char *pcKey);
    //获取节内的配置项的值(char*)
    char *GetItemString(struct St_NSP_PDCSMCS_SectionIni *pstItem, char* pcKey);
    //获取节点
    struct St_NSP_PDCSMCS_SectionIni *GetDbSection(struct St_NSP_PDCSMCS_SectionIni *pstHead,  char *pcSectionName);
    

    //读取数据库配置文件信息
    struct St_NSP_PDCSMCS_SectionIni *  GetDatabaseConfigIni(const char* pfileName);
    //初始化数据库信息
    void InitDataBase(void);
    
    //获取表的总行数
    unsigned int GetPageLines(const char* PageName,unsigned short& RowNumber);
    //获取带有id限制的行数
    unsigned int GetPageLinesLimitId(const char* PageName, const std::string& key, const std::string& value ,unsigned short& RowNumber);




    //链接数据库
    int ConnectToDM();
     //读取配置文件、初始化数据库连接
    int IniDM8SchemaTable();
    //创建数据库模式
    int CreateSchema();
    //创建数据库表
    int CreateTable(int);
    //绑定要执行的sql语句
    int BindSql2Exe(DMStmt& stDmStmt, const char* sql);




    //执行绑定的sql语句并获得数据
    int GetDataByExcuteSql(DMStmt* stDmStmt,PersonnalDataStruct **jsonOUt);
    //设置要返回给前台的结构体信息
    int SetDataValue(St_NSP_PDCSMCS_DataTable **pstTable, const char *ptableName, int rowAll, int iCurrentpage, int& rowStart, int& rowEnd);
    //设置要返回给前台的结构体信息,返回的字段是我们自定义的字段
    int SetDataValueEx(St_NSP_PDCSMCS_DataTable **pstTable, const dataForSearch* dataSearch,const char *ptableName, int rowAll, int iCurrentpage, int& rowStart, int& rowEnd);
    

    
   
    
    
private:
    DMconn m_stDMConnect;   //数据库链接句柄
    struct St_NSP_PDCSMCS_SectionIni *m_pstIniHead; // 将待操作数据进行结构化，得到结构化数据，便于数据库统一操作
    struct St_NSP_PDCSMCS_TableMap *m_pstTableMap;	// 数据库映射表结构体指针,为数据插入数据库提供相应的键名、值类型
    PersonnalDataStruct m_dataAll; //存储数据库查询数据容器
    DataBase_Table_list m_dataList;


};

#endif // SQLOPERATE_H
