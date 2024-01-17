
#ifndef SQLOPERATE_H
#define SQLOPERATE_H

#include "cmdresult.h"
#include <utility>
#include <string>
#include "SimpleIni.h"
#include "LogPrint.h"
#include "DbData.h"
#include <fstream>
#include <sys/stat.h>
extern "C"
{
#include "DMsql.h"
}

#define PATH_DATABASE_CONFIG "/root/CenterExceptional/config/db_config.ini" // 数据库配置文件的地址

class CSqlGradOperate
{
public:
    explicit CSqlGradOperate();
    ~CSqlGradOperate();

public:
    // 初始化数据库信息
    int InitDataBase(void);
    // 按照初始化信息，初始化数据库
    int InitDMShemaAndTable();
    // 将数据存入数据库中
    int DataInsert(const char *pcQuery);
    // 设置数据库的全备份和增量备份
    int SetDbBackup();
    // 按照数据的提取时间和表名查询数据库的数据
    int GetDataBySubmitTime(const char *tableName, const char *timeStart, const char *timeEnd, CDbData &dataAll);
    // 将查询出来的数据以csv文件的格式存到指定的地方
    int SavaDataAsCsv(const CDbData &dataAll, std::string fullName);

private:
    // 绑定要执行的sql语句
    int BindSql2Exe(DMStmt &stDmStmt, const char *sql);
    // 执行绑定的sql语句并获得数据
    int GetDataByExcuteSql(DMStmt *stDmStmt);
    //  获取节内的配置项的值(int)
    int GetItemInt(struct St_NSP_PDCSMCS_SectionIni *pstItem, char *pcKey);
    // 获取节内的配置项的值(char*)
    char *GetItemString(struct St_NSP_PDCSMCS_SectionIni *pstItem, char *pcKey);
    // 获取节点
    struct St_NSP_PDCSMCS_SectionIni *GetDbSection(struct St_NSP_PDCSMCS_SectionIni *pstHead, char *pcSectionName);

    // 读取数据库配置文件信息
    struct St_NSP_PDCSMCS_SectionIni *GetDatabaseConfigIni(const char *pfileName);

    // 获取表的总行数
    unsigned int GetPageLines(const char *PageName, unsigned short &RowNumber);
    // 获取带有id限制的行数
    unsigned int GetPageLinesLimitId(const char *PageName, const std::string &key, const std::string &value, unsigned short &RowNumber);

    // 链接数据库
    int ConnectToDM();
    // 读取配置文件、初始化数据库连接
    int IniDM8SchemaTable();
    // 创建数据库模式
    int CreateSchema();
    // 创建数据库表
    int CreateTable(int);
    // 设置数据库的归档，开启作业
    int SetBackUpDB();
    // 设置完全备份
    int SetCompliBackUp();
    // 设置增量备份
    int SetAddBackUp();
    // 更新完全备份路径
    int UpdateCompliBackUp();
    // 更新增量备份路径
    int UpdateAddBackUp();
    // 初始化查询的数据类
    int InitDbData(const char *tableName, CDbData &data);
    // 组装按照时间查询的查询语句
    int GetSqlByTime(const char *tableName, const char *timeStart, const char *timeEnd, std::string &sql);
    // 获取查询语句的结果
    int GetDataByExcuteSql(DMStmt *stDmStmt, CDbData &data);

private:
    CSimpleIni m_systemIni;
    DMconn m_stDMConnect;                           // 数据库链接句柄
    struct St_NSP_PDCSMCS_SectionIni *m_pstIniHead; // 将待操作数据进行结构化，得到结构化数据，便于数据库统一操作
    struct St_NSP_PDCSMCS_TableMap *m_pstTableMap;  // 数据库映射表结构体指针,为数据插入数据库提供相应的键名、值类型
    std::string m_path;                             // csv保存的路径
};

#endif // SQLOPERATE_H
