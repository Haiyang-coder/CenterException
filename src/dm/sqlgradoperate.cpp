#include "sqlGradoperate.h"
#include "tools/Utils.h"

CSqlGradOperate::CSqlGradOperate()
{
    m_path = m_simpleIni.GetValue("ftp", "CSV_PATH", "/dm8/test");
}

CSqlGradOperate::~CSqlGradOperate()
{
}

/*
 * 功能：获取表的总行数
 * 参数：pageName 表名字 in
 *      RowNumber 总行数 out
 * 返回值：0:success others：failed
 **/
unsigned int CSqlGradOperate::GetPageLines(const char *pageName, unsigned short &RowNumber)
{
    DMStmt stDmStmt = {0};
    std::string strName(pageName);
    std::string acQuery = std::string("select count(*) from " + std::string((const char *)m_pstTableMap->aucSchema) + ".\"") + pageName + std::string("\"");
    int iRet = BindSql2Exe(&stDmStmt, acQuery.c_str());
    if (iRet != SH_LOD_SUCCESS)
    {
        return iRet;
    }

    // bind clomn
    int iRowNumber = 0;
    long long lliRowNumber = 0;
    if (databaseBindColumn(&stDmStmt, 1, DSQL_C_ULONG, &iRowNumber, sizeof(iRowNumber), &lliRowNumber) < 0)
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }

    // fetch data
    if (databaseFetch(&stDmStmt) == DSQL_NO_DATA)
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }
    RowNumber = iRowNumber;
    // close handle
    if (dm_free_stmt(&stDmStmt))
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }

    return 0;
}
unsigned int CSqlGradOperate::GetPageLinesLimitId(const char *PageName, const std::string &key,
                                                  const std::string &value, unsigned short &RowNumber)
{
    DMStmt stDmStmt = {0};
    std::string strName(PageName);
    std::string acQuery = std::string("select count(*) from " + std::string((const char *)m_pstTableMap->aucSchema) + ".\"") + PageName + std::string("\"") + " where " + key + "= " + value;
    int iRet = BindSql2Exe(&stDmStmt, acQuery.c_str());
    if (iRet != SH_LOD_SUCCESS)
    {
        return iRet;
    }

    // bind clomn
    int iRowNumber = 0;
    long long lliRowNumber = 0;
    if (databaseBindColumn(&stDmStmt, 1, DSQL_C_ULONG, &iRowNumber, sizeof(iRowNumber), &lliRowNumber) < 0)
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }

    // fetch data
    if (databaseFetch(&stDmStmt) == DSQL_NO_DATA)
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }
    RowNumber = iRowNumber;
    // close handle
    if (dm_free_stmt(&stDmStmt))
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }

    return 0;
}

unsigned int CSqlGradOperate::GetCommitTimeLines(const char *Start, const char *End, const char *tableName, long &RowNumber)
{
    DMStmt stDmStmt = {0};
    std::string strSql = "SELECT count(*) FROM " + std::string((const char *)m_pstTableMap->aucSchema) + ".\"" +
                         std::string(tableName) +
                         "\" WHERE SUBMITTIME BETWEEN '" +
                         std::string(Start) +
                         "' AND '" + std::string(End) + "';";
    int iRet = BindSql2Exe(&stDmStmt, strSql.c_str());
    if (iRet != SH_LOD_SUCCESS)
    {
        return iRet;
    }

    // bind clomn
    int iRowNumber = 0;
    long long lliRowNumber = 0;
    if (databaseBindColumn(&stDmStmt, 1, DSQL_C_ULONG, &iRowNumber, sizeof(iRowNumber), &lliRowNumber) < 0)
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }

    // fetch data
    if (databaseFetch(&stDmStmt) == DSQL_NO_DATA)
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }
    RowNumber = iRowNumber;
    // close handle
    if (dm_free_stmt(&stDmStmt))
    {
        dm_free_stmt(&stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }

    return 0;
}

/*
 * 功能：连接数据库
 * 参数：无
 * 返回值：
 *		0：成功 其他：失败
 **/
int CSqlGradOperate::ConnectToDM()
{
    std::string strID = "";
    std::string strIP = "";
    std::string strPassword = "";
    int Port = 0;

    strID = m_simpleIni.GetValue("dbInfo", "LOGIN_NAME", "SYSDBA");
    strPassword = m_simpleIni.GetValue("dbInfo", "LOGIN_PASSWORD", "SYSDBA");
    strIP = m_simpleIni.GetValue("dbInfo", "IP", "127.0.0.1");
    Port = std::stoi(m_simpleIni.GetValue("dbInfo", "PORT", "5236"));
    int iRet = dm_connect(&m_stDMConnect, strIP.c_str(), strID.c_str(), strPassword.c_str(), Port);
    if (SH_LOD_SUCCESS != iRet)
    {
        return iRet;
    }

    return iRet;
}

/// @brief 初始化模式和表数据
/// @return
int CSqlGradOperate::IniDM8SchemaTable()
{
    // 检查数据库中是否已有模式Schema
    int iRet = 0;
    unsigned int uiIsSchemaExist = 0;
    DMStmt stDmCheckSchemaHdl = {0};
    char acGetDmSchemaName[D_NSP_PDCSMCS_TICS_BUF_LEN] = {0};
    long long llDmSchemaLen = 0;
    unsigned int uiRow = 0;
    char acSqlCheckSchemaStr[D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN] = {0};
    sprintf(acSqlCheckSchemaStr, "select t.name from sysobjects t where t.type$='SCH' and t.name='%s';", m_pstTableMap->aucSchema);
    do
    {
        iRet = BindSql2Exe(&stDmCheckSchemaHdl, (const char *)acSqlCheckSchemaStr);
        if (0 != iRet)
        {
            std::cout << "数据库 执行错误 BindSql2Exe" << std::endl;
            break;
        }
        if (DSQL_SUCCESS != databaseBindColumn(&stDmCheckSchemaHdl, 1, DSQL_C_NCHAR, acGetDmSchemaName, D_NSP_PDCSMCS_TICS_BUF_LEN + 1, &llDmSchemaLen))
        {
            std::cout << "数据库 执行错误 databaseBindColumn" << std::endl;
            break;
        }

        while (DSQL_NO_DATA != dpi_fetch(stDmCheckSchemaHdl.hstmt, (ulength *)&uiRow))
        {
            std::cout << "数据库已存在模式:" << acGetDmSchemaName << std::endl;
            uiIsSchemaExist = 1;
            break;
        }

    } while (0);
    // 释放语句句柄
    if (dm_free_stmt(&stDmCheckSchemaHdl) != 0)
    {
        std::cout << "释放数据库语句句柄错误" << std::endl;
        return -1;
    }

    // 如果没有创建模式 需要表和模式一同创建
    if (!uiIsSchemaExist)
    {
        iRet = CreateSchema();
        if (0 != iRet)
        {
            std::cout << "创建模式错误！" << std::endl;
            return -1;
        }

        for (int i = 0; i < m_pstTableMap->usSize; i++)
        {
            iRet = CreateTable(i);
            if (0 != iRet)
            {
                std::cout << "创建表错误！" << std::endl;
                return -1;
            }
        }
    }
    // 如果已有模式 有表的情况下 不再创建，无表的情况下进行创建
    else
    {
        int i, iSchemaSize;
        iSchemaSize = m_pstTableMap->usSize;
        for (i = 0; i < iSchemaSize; i++)
        {
            unsigned int uiIsTableExist = 0;
            DMStmt stDmCheckTableHdl = {0}; // 查询当前模式中所有的表信息
            char acGetDmTableName[D_NSP_PDCSMCS_TICS_BUF_LEN] = {0};
            long long llDmTableLen = 0;
            unsigned int uiTalbeRow = 0;
            char acSqlCheckTableStr[D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN] = {0};
            sprintf(acSqlCheckTableStr, "select t.table_name from dba_tables t where t.owner='%s' and t.table_name='%s';", m_pstTableMap->aucSchema, m_pstTableMap->pstTable[i]->aucTableName);

            do
            {
                iRet = BindSql2Exe(&stDmCheckTableHdl, acSqlCheckTableStr);
                if (0 != iRet)
                {
                    std::cout << "数据库执行语句错误:" << std::endl;
                    break;
                }

                if (DSQL_SUCCESS != databaseBindColumn(&stDmCheckTableHdl, 1, DSQL_C_NCHAR, acGetDmTableName, D_NSP_PDCSMCS_TICS_BUF_LEN + 1, &llDmTableLen))
                {
                    std::cout << "绑定列错误:" << std::endl;
                    break;
                }

                while (DSQL_NO_DATA != dpi_fetch(stDmCheckTableHdl.hstmt, (ulength *)&uiTalbeRow))
                {
                    std::cout << "数据库中已存在表:" << acGetDmTableName << std::endl;

                    uiIsTableExist = 1;
                    break;
                }
            } while (0);
            // 释放语句句柄
            if (dm_free_stmt(&stDmCheckTableHdl) != 0)
            {
                std::cout << "释放数据库语句句柄错误:" << std::endl;
                return -1;
            }
            // 如果表不存在的情况下，创建表
            if (!uiIsTableExist)
            {
                iRet = CreateTable(i);
                if (0 != iRet)
                {
                    std::cout << "创建表错误！:" << std::endl;
                    return -1;
                }
            }
            // 如果表存在的情况下， 检查表中是否有列的新增
            // alter table "DEMO2"."DeletePerm_Rate" add column("COLUMN_1" CHAR(10));
            else
            {

                int j, iTableSize = m_pstTableMap->pstTable[i]->usSize;
                for (j = 0; j < iTableSize; j++)
                {
                    unsigned int uiIsColumnExist = 0;
                    DMStmt stDmCheckColumnHdl = {0}; // 查询当前模式中所有的表信息
                    char acGetDmColumnName[D_NSP_PDCSMCS_TICS_BUF_LEN] = {0};
                    long long llDmColumnLen = 0;
                    unsigned int uiColumnRow = 0;
                    char acSqlCheckColumnStr[D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN] = {0};

                    memset(acSqlCheckColumnStr, 0, sizeof(acSqlCheckColumnStr));
                    sprintf(acSqlCheckColumnStr, "select t.column_name from dba_tab_columns t where t.table_name='%s' and t.column_name= UPPER('%s');", m_pstTableMap->pstTable[i]->aucTableName, m_pstTableMap->pstTable[i]->astMap[j].aucColumnName);

                    do
                    {
                        iRet = BindSql2Exe(&stDmCheckColumnHdl, acSqlCheckColumnStr);
                        if (0 != iRet)
                        {
                            std::cout << "数据库执行语句错误:" << std::endl;
                            break;
                        }

                        if (DSQL_SUCCESS != databaseBindColumn(&stDmCheckColumnHdl, 1, DSQL_C_NCHAR, acGetDmColumnName, D_NSP_PDCSMCS_TICS_BUF_LEN + 1, &llDmColumnLen))
                        {
                            std::cout << "绑定列错误:" << std::endl;
                            break;
                        }

                        while (DSQL_NO_DATA != dpi_fetch(stDmCheckColumnHdl.hstmt, (ulength *)&uiColumnRow))
                        {
                            // DBG_PRINT("数据库%s表中已存在列%s\n", m_pstTableMap->pstTable[i]->aucTableName, acGetDmColumnName);
                            uiIsColumnExist = 1;
                            break;
                        }
                    } while (0);
                    // 释放语句句柄
                    if (dm_free_stmt(&stDmCheckColumnHdl) != 0)
                    {

                        std::cout << "释放数据库语句句柄错误:" << std::endl;
                        return -1;
                    }
                    // 如果该表中列不存在的情况下，添加该列到该表中
                    if (!uiIsColumnExist)
                    {
                        DMStmt stAlertColumnHdl = {0};
                        char acAlertColumnSql[D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN] = {0};
                        sprintf(acAlertColumnSql, "alter table \"%s\".\"%s\" add column(%s %s);", m_pstTableMap->aucSchema, m_pstTableMap->pstTable[i]->aucTableName, m_pstTableMap->pstTable[i]->astMap[j].aucColumnName, m_pstTableMap->pstTable[i]->astMap[j].aucType);
                        std::cout << "acAlertColumnSql:" << acAlertColumnSql << std::endl;

                        iRet = BindSql2Exe(&stAlertColumnHdl, acAlertColumnSql);
                        if (0 != iRet)
                        {
                            std::cout << "数据库执行语句错误:" << std::endl;
                            break;
                        }
                        std::cout << "在表:" << m_pstTableMap->pstTable[i]->aucTableName << "新增：" << m_pstTableMap->pstTable[i]->astMap[j].aucColumnName << "成功" << std::endl;
                        // 释放语句句柄
                        if (dm_free_stmt(&stAlertColumnHdl) != 0)
                        {
                            std::cout << "释放数据库语句句柄错误:" << std::endl;
                            return -1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/// @brief  创建数据库模式语句
/// @return
int CSqlGradOperate::CreateSchema()
{
    // 创建数据库模式语句
    // CREATE SCHEMA "DEMO" AUTHORIZATION "SYSDBA";
    DMStmt stDmHdl = {0};
    char acSqlStr[D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN] = {0};
    char acSchemaName[D_NSP_PDCSMCS_TICS_BUF_LEN] = {0};
    char *pcSqlFmt = "CREATE SCHEMA \"%s\" AUTHORIZATION \"%s\";";

    sprintf(acSchemaName, "%s", (char *)m_pstTableMap->aucSchema);
    sprintf(acSqlStr, pcSqlFmt, m_pstTableMap->aucSchema, m_pstTableMap->aucAuthorization);
    std::cout << "acSqlString 创建模式的语句是：" << acSqlStr << std::endl;

    // 执行sql语句
    int iRet = BindSql2Exe(&stDmHdl, acSqlStr);
    if (0 != iRet)
    {
        std::cout << "数据库执行语句错误" << std::endl;
        return -1;
    }
    std::cout << "创建数据库模式成功:" << acSchemaName << std::endl;

    // 释放语句句柄
    if (dm_free_stmt(&stDmHdl) != 0)
    {
        std::cout << "释放数据库语句句柄错误" << std::endl;
        return -1;
    }

    return 0;
}

int CSqlGradOperate::CreateTable(int i)
{
    char *pcSqlStr = NULL;
    pcSqlStr = (char *)malloc(sizeof(char) * D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN);
    int j, iOffset, iRet, iTableSize;
    char acCreateTable[D_NSP_PDCSMCS_TICS_BUF_LEN]; // 拼接的模式.表信息
    char acColumnAttr[D_NSP_PDCSMCS_TICS_BUF_LEN];  // 拼接的表中列的信息
    DMStmt stDmHdlCreateTable = {0};                // 创建数据库表语句

    iOffset = 0;
    memset(pcSqlStr, 0, D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN);
    memset(acCreateTable, 0, sizeof(acCreateTable));
    sprintf(acCreateTable, "CREATE TABLE \"%s\".\"%s\" (", m_pstTableMap->aucSchema, m_pstTableMap->pstTable[i]->aucTableName);
    memcpy(pcSqlStr, acCreateTable, strlen(acCreateTable));
    iOffset += strlen(acCreateTable);

    memset(acColumnAttr, 0, sizeof(acColumnAttr));
    iTableSize = m_pstTableMap->pstTable[i]->usSize;
    for (j = 0; j < iTableSize - 1; j++)
    {
        memset(acColumnAttr, 0, sizeof(acColumnAttr));
        // 修改为自动大写
        sprintf(acColumnAttr, "%s %s,", m_pstTableMap->pstTable[i]->astMap[j].aucColumnName, m_pstTableMap->pstTable[i]->astMap[j].aucType);
        memcpy(pcSqlStr + iOffset, acColumnAttr, strlen(acColumnAttr));
        iOffset += strlen(acColumnAttr);
    }
    memset(acColumnAttr, 0, sizeof(acColumnAttr));
    // 修改为自动大写
    sprintf(acColumnAttr, "%s %s);", m_pstTableMap->pstTable[i]->astMap[j].aucColumnName, m_pstTableMap->pstTable[i]->astMap[j].aucType);
    memcpy(pcSqlStr + iOffset, acColumnAttr, strlen(acColumnAttr));
    iOffset += strlen(acColumnAttr);
    std::cout << "pcSqlStr:" << pcSqlStr << std::endl;

    // 执行sql语句
    iRet = BindSql2Exe(&stDmHdlCreateTable, pcSqlStr);
    if (0 != iRet)
    {
        std::cout << "数据库执行语句错误:" << std::endl;
        return -1;
    }
    std::cout << "创建数据库表:" << m_pstTableMap->pstTable[i]->aucTableName << std::endl;
    // 释放语句句柄
    if (dm_free_stmt(&stDmHdlCreateTable) != 0)
    {

        std::cout << "释放数据库语句句柄错误:" << std::endl;
        return -1;
    }

    return 0;
}

/// @brief 初始化数据库的表和模式，没有的话就新建表和模式
/// @return
int CSqlGradOperate::InitDMShemaAndTable()
{
    int ret = 0;
    ret = m_dmTableInfo.IniDmInfo();
    if (ret < 0)
    {
        LOG_ERROR << "m_dmTableInfo 初始化失败";
        return -1;
    }
    m_pstIniHead = m_dmTableInfo.GetModelStruct();
    if (m_pstIniHead == NULL)
    {
        LOG_ERROR << "m_pstIniHead为空";
        return -2;
    }
    m_pstTableMap = m_dmTableInfo.GetTableMap();
    if (m_pstTableMap == NULL)
    {
        LOG_ERROR << "配置文件读取出现问题";
        return -3;
    }
    ret = ConnectToDM();
    if (ret < 0)
    {
        LOG_ERROR << "连接数据库失败";
        return -4;
    }
    ret = IniDM8SchemaTable();
    if (ret < 0)
    {
        LOG_ERROR << "初始化数据库失败";
        return -5;
    }
    return ret;
}

/*
 * 功能：绑定sql语句
 * 参数：
 *		@stDmStmt[in]:db句柄
 *		@pcSectionName[sql]:sql语句
 * 返回值：
 *		@int[out]:0成功 其他失败
 **/
int CSqlGradOperate::BindSql2Exe(DMStmt *stDmStmt, const char *sql)
{
    int iRet = 0; // 返回值
    if (m_stDMConnect.conFlag != 1)
    {
        // 没有连接到数据库
        std::cout << "连接句柄有误：没有连接数据库" << std::endl;
        // 初始化数据库操作
        std::cout << "reconnect to dm..." << std::endl;
        iRet = ConnectToDM();
        if (0 != iRet)
        {
            std::cout << "重新连接数据库失败" << std::endl;
            return -1;
        }
    }
    // get the handle
    if (dm_alloc_stmt(&m_stDMConnect, stDmStmt) < 0)
    {
        // INFO_PRINT << "get the handle failed";
        dm_free_stmt(stDmStmt);
        return SH_SQL_GETHANDLE_ERROR;
    }

    // bind the data
    DPIRETURN ret = databasePrepare(stDmStmt, const_cast<const char *>(sql));
    if (ret < 0)
    {
        // INFO_PRINT << "bind the data failed";
        dm_free_stmt(stDmStmt);
        return ret;
    }

    // execute the sql
    ret = databaseExecuteDirect(stDmStmt, sql);
    if (ret < 0)
    {
        // INFO_PRINT << "execute the sql failed";
        dm_free_stmt(stDmStmt);
        return ret;
    }
    return SH_LOD_SUCCESS;
}

int CSqlGradOperate::BindSql2ExeEx(DMStmt *stDmStmt, const char *sql)
{
    int iRet = 0; // 返回值
    if (m_stDMConnect.conFlag != 1)
    {
        // 没有连接到数据库
        std::cout << "连接句柄有误：没有连接数据库" << std::endl;
        // 初始化数据库操作
        std::cout << "reconnect to dm..." << std::endl;
        iRet = ConnectToDM();
        if (0 != iRet)
        {
            std::cout << "重新连接数据库失败" << std::endl;
            return -1;
        }
    }
    // get the handle
    if (dm_alloc_stmt(&m_stDMConnect, stDmStmt) < 0)
    {
        // INFO_PRINT << "get the handle failed";
        dm_free_stmt(stDmStmt);
        return SH_SQL_GETHANDLE_ERROR;
    }

    // execute the sql
    int ret = databaseExecuteDirect(stDmStmt, sql);
    if (ret < 0)
    {
        // INFO_PRINT << "execute the sql failed";
        // dm_free_stmt(stDmStmt);
        return ret;
    }
    return SH_LOD_SUCCESS;
}

/*
 *功能：实现数据库的插入
 *参数：
 *      @pcQuery[in]:插入语句指针
 *      @pstInfo[in]:Data_Info结构体指针
 *      @usTmpMonClass[in]:采集信息类别
 *返回值：
 *      ERR_OK:成功
 *      -D_NSP_PDCSMCS_TICS_ALLOC_ERR：获取语句句柄错误
 *      -D_NSP_PDCSMCS_TICS_PREPARE_ERR：数据参数准备错误
 *      -D_NSP_PDCSMCS_TICS_INSERT_ERR：执行插入错误
 *      -D_NSP_PDCSMCS_TICS_DATABASE_FREE_FAIL_ERR：数据库释放失败
 * */
int CSqlGradOperate::DataInsert(const char *pcQuery)
{
    DMStmt stDmStmt = {0};
    int ret = BindSql2Exe(&stDmStmt, pcQuery);
    return ret;
}

int CSqlGradOperate::SetDbBackup()
{
    int ret = -1;
    //  设置数据库的归档，开启作业
    DMStmt stDmStmt;
    ret = SetBackUpDB(&stDmStmt);
    if (ret != 0)
    {
        return ret;
    }
    //  设置完全备份
    ret = SetCompliBackUp(&stDmStmt);
    if (ret != 0)
    {
        return ret;
    }
    // 设置增量备份
    ret = SetAddBackUp(&stDmStmt);
    if (ret != 0)
    {
        return ret;
    }
    return ret;
}

/// @brief
/// @param tableName 表名字
/// @param timeStart 数据提交时间（开始）
/// @param timeEnd 数据提交时间（结束）
/// @param dataAll 出参 查询出来的结果
/// @return 0成功
int CSqlGradOperate::GetDataBySubmitTime(const char *tableName, const char *timeStart, const char *timeEnd, CDbData &dataAll)
{
    DMStmt stDmStmt; // 本次数据库的操作句柄
    // 初始化要查询的数据信息（得知行数，列数，列明，列数据类型）
    int ret = InitDbData(tableName, dataAll);
    if (ret < 0)
    {
        LOG_ERROR << "初始化查询数据类，失败";
    }
    // 获取行数
    ret = GetCommitTimeLines(timeStart, timeEnd, tableName, dataAll.iLineSize);
    if (ret < 0)
    {
        LOG_ERROR << "初始化查询数据类，失败";
    }
    //   组装查询语句
    std::string ccSqlselectAll;
    GetSqlByTime(tableName, timeStart, timeEnd, ccSqlselectAll);
    // 执行查询语句
    ret = BindSql2Exe(&stDmStmt, ccSqlselectAll.c_str());
    if (ret != SH_LOD_SUCCESS)
    {
        LOG_ERROR << "执行查询语句失败了";
        return ret;
    }
    // 获取查询的结果
    ret = GetDataByExcuteSql(&stDmStmt, dataAll);
    if (ret != SUCCESS)
    {
        LOG_ERROR << "获取查询的结果失败了";
        return FAILED;
    }
    return SUCCESS;
}

/// @brief 将数据以csv的格式存到指定的文件夹
/// @param dataAll 入参 数据
/// @param fullName 入参 保存的路径（包括文件名）
/// @return 0成功
int CSqlGradOperate::SavaDataAsCsv(const CDbData &dataAll, std::string &fullName)
{

    int lineSize = dataAll.iLineSize; // 行数
    int cloSize = dataAll.iClomuSize; // 列数
    // 获取输出路径的目录部分
    size_t lastSlashPos = fullName.find_last_of('/');
    std::string outputDir = (lastSlashPos != std::string::npos) ? fullName.substr(0, lastSlashPos) : "";

    // 打开文件操作流开始写入数据
    std::ofstream csvFile(fullName);

    // 先写入标题信息
    for (size_t i = 0; i < cloSize; i++)
    {
        if (i == cloSize - 1)
        {
            csvFile << dataAll.vecCloName.at(i) << "\n";
        }
        else
        {
            csvFile << dataAll.vecCloName.at(i) << ",";
        }
    }
    if (lineSize == 0)
    {
        // 关闭文件流
        csvFile.close();
        return 0;
    }
    // 按照数据类型写入剩余的所有数据
    for (size_t i = 0; i < lineSize; i++)
    {
        for (size_t j = 0; j < cloSize; j++)
        {
            LOG_INFO << "vecType.size() = " << dataAll.vecType.size();
            LOG_INFO << "此时的i = " << i << "此时的j=" << j;
            if (dataAll.vecType.at(j) == TYPE_INT)
            {

                if (j == cloSize - 1)
                {
                    csvFile << dataAll.vecDbRow.at(i * cloSize + j).piData << "\n";
                }
                else
                {
                    csvFile << dataAll.vecDbRow.at(i * cloSize + j).piData << ",";
                }
            }
            else if (dataAll.vecType.at(j) == TYPE_STRING)
            {
                if (j == cloSize - 1)
                {
                    csvFile << "\"" << dataAll.vecDbRow.at(i * cloSize + j).strData << "\""
                            << "\n";
                }
                else
                {
                    csvFile << "\"" << dataAll.vecDbRow.at(i * cloSize + j).strData << "\""
                            << ",";
                }
            }
        }
    }
    // 关闭文件流
    csvFile.close();
    return 0;
}
const St_NSP_PDCSMCS_TableMap *CSqlGradOperate::GetTableMap()
{
    if (m_pstTableMap != NULL)
    {
        return m_pstTableMap;
    }
    else
    {
        return nullptr;
    }
}

int CSqlGradOperate::GetModelName(std::string &modelName)
{
    if (m_pstIniHead != NULL)
    {
        modelName = std::string((char *)m_pstTableMap->aucSchema);
    }
    return 0;
}

int CSqlGradOperate::SetBackUpDB(DMStmt *stDmStmt)
{
    int ret = 0;
    // 1.将数据库设置为挂起状态
    std::string strSetMount = "alter database mount;";
    ret = BindSql2ExeEx(stDmStmt, strSetMount.c_str());
    if (ret < 0 && ret != -510)
    {
        LOG_INFO << "将数据库设置为挂起状态 失败";
        return ret;
    }

    // 2.开启数据库的归档配置
    std::string strArchivelog = "alter database archivelog;";
    ret = BindSql2ExeEx(stDmStmt, strArchivelog.c_str());
    if (ret < 0)
    {
        LOG_INFO << "开启数据库的归档配置 失败";
        return ret;
    }

    // 3.配置数据库的归档参数
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpLogDir", "/dm8/backupUnExcept");
    utils::checkOrCreateDirectory(strPath);
    std::string strLogConfig = "alter database add archivelog 'type=local,dest=" + strPath + ",file_size=1024,space_limit=4096';";
    ret = BindSql2ExeEx(stDmStmt, strLogConfig.c_str());
    if (ret < 0)
    {
        if (ret != -806)
        {
            LOG_INFO << "配置数据库的归档参数 失败";
            return ret;
        }
    }

    // 4.修改回open状态
    std::string strSetOpen = "alter database open;";
    ret = BindSql2ExeEx(stDmStmt, strSetOpen.c_str());
    if (ret < 0 && ret != -514)
    {
        LOG_INFO << "修改回open状态 失败";
        return ret;
    }
}

/// @brief 数据库设置了每周一次的全备份
/// @return
int CSqlGradOperate::SetCompliBackUp(DMStmt *stDmStmt)
{
    int ret = 0;
    bool flag = false;
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");

    std::string sql = "call SP_CREATE_JOB('BackUp_center',1,0,'',0,0,'',0,'');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        if (ret == -8413)
        {
            return AlterCompliBackUp(stDmStmt);
        }
        return ret;
    }
    sql = "call SP_JOB_CONFIG_START('BackUp_center');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_STEP('BackUp_center', 'BackUp_dm', 6, '00000000" + strPath + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_SCHEDULE('BackUp_center', 'test1', 1, 2, 1, 64, 0, '00:00:00', NULL, '2024-01-02 23:48:24', NULL, '');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_JOB_CONFIG_COMMIT('BackUp_center');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
    return SUCCESS;
}

int CSqlGradOperate::AlterCompliBackUp(DMStmt *stDmStmt)
{
    // 修改完全备份信息
    int ret = 0;
    bool flag = false;
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");
    std::string sql = "call SP_JOB_CONFIG_START('BackUp_center');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_STEP('BackUp_center', 'BackUp_dm', 6, '00000000/" + strPath + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_SCHEDULE('BackUp_center', 'test1', 1, 2, 1, 64, 0, '00:00:00', NULL, '2024-01-02 23:48:24', NULL, '');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_JOB_CONFIG_COMMIT('BackUp_center');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
    return SUCCESS;
}

int CSqlGradOperate::AlterAddBackUp(DMStmt *stDmStmt)
{
    int ret = 0;
    std::string strPathAdd = m_simpleIni.GetValue("dbInfo", "BackAddDir", "/dm8/backup");
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");

    std::string sql = "call SP_JOB_CONFIG_START('BackUp_Add');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_STEP('BackUp_Add', 'addbackup', 6, '10000000" + strPath + "|" + strPathAdd + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_SCHEDULE('BackUp_Add', 'BackUp_Add', 1, 2, 1, 1, 0, '21:39:38', NULL, '2024-03-06 21:39:38', NULL, '');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_JOB_CONFIG_COMMIT('BackUp_Add');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
}

/// @brief 给数据库设置每天一次的增量备份
/// @return
int CSqlGradOperate::SetAddBackUp(DMStmt *stDmStmt)
{
    int ret = 0;
    std::string strPathAdd = m_simpleIni.GetValue("dbInfo", "BackAddDir", "/dm8/backup");
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");

    std::string sql = "call SP_CREATE_JOB('BackUp_Add',1,0,'',0,0,'',0,'');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        if (ret == -8413)
        {

            return AlterAddBackUp(stDmStmt);
        }
        return ret;
    }

    sql = "call SP_JOB_CONFIG_START('BackUp_Add');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_STEP('BackUp_Add', 'addbackup', 6, '10000000" + strPath + "|" + strPathAdd + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_SCHEDULE('BackUp_Add', 'BackUp_Add', 1, 2, 1, 64, 0, '00:00:00', NULL, '2024-01-02 23:48:24', NULL, '');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_JOB_CONFIG_COMMIT('BackUp_Add');";
    ret = BindSql2ExeEx(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
}

int CSqlGradOperate::UpdateCompliBackUp()
{
    int ret = 0;
    DMStmt stDmStmt;
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");
    std::string sql = "call SP_ALTER_JOB_STEP('BackUp_center', 'BackUp_dm', 6, '00000000/" + strPath + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2ExeEx(&stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
    return SUCCESS;
}

int CSqlGradOperate::UpdateAddBackUp()
{
    int ret = 0;
    DMStmt stDmStmt;
    std::string strPathAdd = m_simpleIni.GetValue("dbInfo", "BackUpAddDir", "/dm8/backup");
    std::string strPath = m_simpleIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");
    return SUCCESS;

    std::string sql = "call SP_JOB_CONFIG_START('BackUp_Add');";
    ret = BindSql2ExeEx(&stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_STEP('BackUp_Add', 'Add_test_buzhou', 6, '10000000" + strPath + "|" + strPathAdd + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2ExeEx(&stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
    return SUCCESS;
}

int CSqlGradOperate::InitDbData(const char *tableName, CDbData &data)
{
    St_NSP_PDCSMCS_DataTable *tableTemp; // 对应表的信息
    //  在m_pstTableMap中查找对应的表
    for (int i = 0; i < m_pstTableMap->usSize; i++)
    {
        if (strcmp((char *)(m_pstTableMap->pstTable[i]->aucTableName), tableName) == 0)
        {
            tableTemp = m_pstTableMap->pstTable[i];
            break;
        }
        if (m_pstTableMap->usSize == i)
        {
            LOG_ERROR << "没有找到指定的表名";
            return -1;
        }
    }

    // 3. 获得表的列数,以及每一列的数据类型
    data.iClomuSize = tableTemp->usSize;
    // 获得所有列的名字，以及他们的数据类型
    for (int i = 0; i < tableTemp->usSize; i++)
    {
        data.vecCloName.push_back(std::string((char *)tableTemp->astMap[i].aucColumnName));
        if (tableTemp->astMap[i].usType == TYPE_INT)
        {
            data.vecType.push_back(TYPE_INT);
        }
        else
        {
            data.vecType.push_back(TYPE_STRING);
        }
    }
    return 0;
}

/// @brief 获得按照时间范围查找数据的sql语句
/// @param tableName
/// @param timeStart
/// @param timeEnd
/// @param sql
/// @return
int CSqlGradOperate::GetSqlByTime(const char *tableName, const char *timeStart, const char *timeEnd, std::string &sql)
{
    std::string strSql = "SELECT * FROM " + std::string((const char *)m_pstTableMap->aucSchema) + ".\"" +
                         std::string(tableName) +
                         "\" WHERE SUBMITTIME BETWEEN '" +
                         std::string(timeStart) +
                         "' AND '" + std::string(timeEnd) + "';";
    sql = strSql;
    LOG_INFO << "组合完毕的sql是:" + strSql;
    return 0;
}

/// @brief 将查询结果取出来
/// @param stmt 本粗查询使用的句柄
/// @param data 出参 将结果都放到这个容器里面
/// @return 0成功
int CSqlGradOperate::GetDataByExcuteSql(DMStmt *stDmStmt, CDbData &data)
{
    // 开始为数据申请空间
    int iRet = 0;
    int *piData = nullptr;
    piData = (int *)malloc(sizeof(int) * data.iClomuSize);
    memset(piData, 0, sizeof(int) * data.iClomuSize);
    char **ppcData = (char **)malloc(sizeof(char *) * data.iClomuSize);
    for (int i = 0; i < data.iClomuSize; i++)
    {
        ppcData[i] = (char *)malloc(sizeof(char) * MAX_SIZE);
        memset(ppcData[i], 0, sizeof(char) * MAX_SIZE);
    }
    long long *pllBindClomn = (long long *)malloc(data.iClomuSize * sizeof(long long));
    for (int inumber = 0; inumber < data.iClomuSize; inumber++)
    {
        if (data.vecType.at(inumber) == TYPE_INT)
        {
            iRet = databaseBindColumn(stDmStmt, inumber + 1, DSQL_C_SLONG, &piData[inumber],
                                      sizeof(piData[inumber]), &pllBindClomn[inumber]);
        }
        else if (data.vecType.at(inumber) == TYPE_STRING)
        {
            iRet = databaseBindColumn(stDmStmt, inumber + 1, DSQL_C_NCHAR, ppcData[inumber],
                                      MAX_SIZE, &pllBindClomn[inumber]);
        }
        if (iRet < 0)
        {
            dm_free_stmt(stDmStmt);
            return SH_SQL_BINDCLOMN_ERROR;
        }
    }

    // get data
    for (int i = 0; i < data.iLineSize; i++)
    {
        if (databaseFetch(stDmStmt) == DSQL_NO_DATA)
        {
            break;
        }
        for (int inumber = 0; inumber < data.iClomuSize; inumber++)
        {
            if (data.vecType.at(inumber) == TYPE_INT)
            {
                CDbRowData dataRow(piData[inumber]);
                data.vecDbRow.push_back(dataRow);
            }
            else if (data.vecType.at(inumber) == TYPE_STRING)
            {
                CDbRowData dataRow(ppcData[inumber]);
                data.vecDbRow.push_back(dataRow);
            }
        }
    }
    // close handle
    if (dm_free_stmt(stDmStmt))
    {
        dm_free_stmt(stDmStmt);
        return SH_SQL_BINDCLOMN_ERROR;
    }
    delete[] ppcData;
    delete[] piData;
    return 0;
}
