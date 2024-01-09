#include "sqlGradoperate.h"
#include "unistd.h"
#include <string>
#include <iostream>

CSqlGradOperate::CSqlGradOperate()
{
    // 定义ini文档对象
    // 参数为true时保存为UTF-8格式，否则为本地编码格式
    m_systemIni.SetUnicode(true);
    // 是否允许一个关键字对应多个值，默认为允许；若不允许，则将最后一个值作为此关键字关联的值
    m_systemIni.SetMultiKey(false);
    SI_Error ret = 0;
    ret = m_systemIni.LoadFile(SYSTEM_INI_FILE_PATH);
    if (ret < 0)
    {
        std::cout << "加载文件：" << SYSTEM_INI_FILE_PATH << "失败了" << std::endl;
        exit(ret);
    }
    // 将配置文件中的配置读进内存
    m_pstIniHead = GetDatabaseConfigIni(PATH_DATABASE_CONFIG);
    // 开始按照配置初始化数据库
    InitDataBase();
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
    DMStmt stDmStmt;
    std::string strName(pageName);
    std::string acQuery = std::string("select count(*) from " + std::string((const char *)m_pstTableMap->aucSchema) + ".\"") + pageName + std::string("\"");
    int iRet = BindSql2Exe(stDmStmt, acQuery.c_str());
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
    DMStmt stDmStmt;
    std::string strName(PageName);
    std::string acQuery = std::string("select count(*) from " + std::string((const char *)m_pstTableMap->aucSchema) + ".\"") + PageName + std::string("\"") + " where " + key + "= " + value;
    int iRet = BindSql2Exe(stDmStmt, acQuery.c_str());
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

    strID = m_systemIni.GetValue("dbInfo", "LOGIN_NAME", "SYSDBA");
    strPassword = m_systemIni.GetValue("dbInfo", "LOGIN_PASSWORD", "SYSDBA");
    strIP = m_systemIni.GetValue("dbInfo", "IP", "127.0.0.1");
    Port = std::stoi(m_systemIni.GetValue("dbInfo", "PORT", "5236"));
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
        iRet = BindSql2Exe(stDmCheckSchemaHdl, (const char *)acSqlCheckSchemaStr);
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
                iRet = BindSql2Exe(stDmCheckTableHdl, acSqlCheckTableStr);
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
                        iRet = BindSql2Exe(stDmCheckColumnHdl, acSqlCheckColumnStr);
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

                        iRet = BindSql2Exe(stAlertColumnHdl, acAlertColumnSql);
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
    int iRet = BindSql2Exe(stDmHdl, acSqlStr);
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
    iRet = BindSql2Exe(stDmHdlCreateTable, pcSqlStr);
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
    if (m_pstTableMap == NULL)
    {
        std::cout << "配置文件读取出现问题" << std::endl;
        return -1;
    }
    ret = ConnectToDM();
    if (ret < 0)
    {
        std::cout << "ConnectToDM failed" << std::endl;
        return ret;
    }
    ret = IniDM8SchemaTable();
    if (ret < 0)
    {
        std::cout << "IniDM8SchemaTable failed" << std::endl;
        return ret;
    }
    return ret;
}

/*
 * 功能：初始化数据库信息
 * 参数：无
 * 返回值：无
 **/
void CSqlGradOperate::InitDataBase(void)
{
    int iTableSize = 0;
    char acTableName[D_NSP_PDCSMCS_TICS_CLOMN_NAME_LEN] = {0};
    char acColumnName[D_NSP_PDCSMCS_TICS_CLOMN_NAME_LEN] = {0};
    unsigned short iColumnSize = 0;
    struct St_NSP_PDCSMCS_SectionIni *pstIniTable = NULL;
    struct St_NSP_PDCSMCS_SectionIni *pstIniSchema = GetDbSection(m_pstIniHead, "schema");

    if (pstIniSchema == NULL)
    {
        printf("未找到schema节\n");
        exit(0);
    }

    // 获取表的数目
    iTableSize = GetItemInt(pstIniSchema, "SIZE");

    m_pstTableMap = (struct St_NSP_PDCSMCS_TableMap *)malloc(sizeof(struct St_NSP_PDCSMCS_TableMap) + sizeof(struct St_NSP_PDCSMCS_DataTable *) * iTableSize);
    strcpy((char *)m_pstTableMap->aucSchema, GetItemString(pstIniSchema, "SCHEMA"));
    strcpy((char *)m_pstTableMap->aucAuthorization, GetItemString(pstIniSchema, "AUTHORIZATION"));
    // sprintf(m_pstTableMap->aucSchema, "%s", GetItemString(pstIniSchema, "SCHEMA"));
    // sprintf(m_pstTableMap->aucAuthorization, "%s", GetItemString(pstIniSchema, "AUTHORIZATION"));
    m_pstTableMap->usSize = iTableSize;
    for (int i = 0; i < iTableSize; i++)
    {
        sprintf(acTableName, "%s%d", "TABLENAME", i);

        // 获取表
        if ((pstIniTable = GetDbSection(m_pstIniHead, GetItemString(pstIniSchema, acTableName))) == NULL)
        {
            printf("%s未找到%s配置项", "schema", acTableName);
            exit(0);
        }

        iColumnSize = (unsigned short)GetItemInt(pstIniTable, "SIZE");

        /**
         * 为表结构申请空间并赋值
         */
        struct St_NSP_PDCSMCS_DataTable *pstDataTable = (struct St_NSP_PDCSMCS_DataTable *)malloc(sizeof(struct St_NSP_PDCSMCS_DataTable) + sizeof(struct Data_Map) * iColumnSize);
        pstDataTable->usSize = iColumnSize;
        sprintf((char *)(pstDataTable->aucTableName), "%s", GetItemString(pstIniSchema, acTableName));

        // 循环获取表的列以及列的属性
        for (int j = 0; j < iColumnSize; j++)
        {
            struct Data_Map *pstDataMap = (struct Data_Map *)pstDataTable->astMap + j;
            sprintf(acColumnName, "%s%d", "COLUMN", j);
            if (sscanf(GetItemString(pstIniTable, acColumnName), "%[^#]%*[#]%s", (char *)(pstDataMap->aucColumnName), (char *)(pstDataMap->aucType)) != 2)
            {
                printf("F_NSP_PDCSMCS_ini_get_string(pstIniTable,acColumnName)配置参数不正确\n");
                return;
            }
            if ((strncmp((const char *)pstDataMap->aucType, "VARCHAR", 7) == 0) || (strncmp((const char *)pstDataMap->aucType, "CHAR", 4) == 0))
            {
                pstDataMap->usType = 4;
            }
            else if ((strncmp((const char *)pstDataMap->aucType, "INT", 3) == 0))
            {
                pstDataMap->usType = 2;
            }
            else
            {
            }

            // DBG_PRINT("%s %s\n",pstMap->aucColumnName,pstMap->aucType);
        }
        (m_pstTableMap->pstTable)[i] = pstDataTable;
        // printf("pstTable: %s\n",pstTable);
    }
}
/*
 * 功能：获取指定的节
 * 参数：
 *		@pstHead[in]:节头指针
 *		@pcSectionName[in]:待获取节名
 * 返回值：
 *		@pstItem[out]:指定节
 **/
St_NSP_PDCSMCS_SectionIni *CSqlGradOperate::GetDbSection(St_NSP_PDCSMCS_SectionIni *pstHead, char *pcSectionName)
{
    struct St_NSP_PDCSMCS_SectionIni *pstItem = pstHead;
    while (pstItem != NULL && strcmp(pstItem->acSectionName, pcSectionName))
    {
        pstItem = pstItem->pstNextSection;
    }
    return pstItem;
}

/*
 * 功能：绑定sql语句
 * 参数：
 *		@stDmStmt[in]:db句柄
 *		@pcSectionName[sql]:sql语句
 * 返回值：
 *		@int[out]:0成功 其他失败
 **/
int CSqlGradOperate::BindSql2Exe(DMStmt &stDmStmt, const char *sql)
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
    if (dm_alloc_stmt(&m_stDMConnect, &stDmStmt) < 0)
    {
        // INFO_PRINT << "get the handle failed";
        dm_free_stmt(&stDmStmt);
        return SH_SQL_GETHANDLE_ERROR;
    }

    // bind the data
    DPIRETURN ret = databasePrepare(&stDmStmt, const_cast<const char *>(sql));
    if (ret < 0)
    {
        // INFO_PRINT << "bind the data failed";
        dm_free_stmt(&stDmStmt);
        return ret;
    }

    // execute the sql
    ret = databaseExecuteDirect(&stDmStmt, sql);
    if (ret < 0)
    {
        // INFO_PRINT << "execute the sql failed";
        dm_free_stmt(&stDmStmt);
        return ret;
    }

    // 释放语句句柄
    if (dm_free_stmt(&stDmStmt) != 0)
    {
        return -D_NSP_PDCSMCS_TICS_DATABASE_FREE_FAIL_ERR;
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
    DMStmt stDmStmt;
    int ret = BindSql2Exe(stDmStmt, pcQuery);
    return ret;
}

/*
 * 功能：绑定sql语句
 * 参数：
 *		@pfileName[in]:数据库表名字
 *		@pcSectionName[sql]:sql语句
 * 返回值：
 *		@int[out]:数据库配置结构体
 **/
struct St_NSP_PDCSMCS_SectionIni *CSqlGradOperate::GetDatabaseConfigIni(const char *pfileName)
{
    int i = 0;
    struct St_NSP_PDCSMCS_SectionIni *pstHead = NULL;
    struct St_NSP_PDCSMCS_SectionIni *pstItem = NULL;
    struct Key_Value *pstKeyValue = NULL;
    char *pcBuf = (char *)malloc(g_read_line_maxsize + 1);
    memset(pcBuf, 0, g_read_line_maxsize);
    // 打开文件
    FILE *hIniFile = fopen(pfileName, "r");
    if (hIniFile == NULL)
    {
        return NULL;
    }

    while (NULL != fgets(pcBuf, g_read_line_maxsize, hIniFile))
    {
        int buff_len = strlen(pcBuf);
        if (pcBuf[buff_len - 1] != '\n' && !feof(hIniFile))
        {
            printf("单行长度过长!\n");
            return NULL;
        }

        i++;
        if (isalpha(*pcBuf))
        {
            // 键值对
            pstKeyValue = (struct Key_Value *)malloc(sizeof(struct Key_Value));

            if (sscanf(pcBuf, "%[^= ]%*[ =]%s", (char *)pstKeyValue->acKey, (char *)pstKeyValue->acValue) != 2)
            {
                printf("无法解析 %s %d行\n", pfileName, i);
                exit(0);
            };
            pstKeyValue->pstNextKeyValue = pstItem->pstKeyValue;
            pstItem->pstKeyValue = pstKeyValue;
            // printf("alpha %s=%s\n",pstKeyValue->acKey,pstKeyValue->acValue);
        }
        else if (*pcBuf == '[')
        {
            // Section
            pstItem = (struct St_NSP_PDCSMCS_SectionIni *)malloc(sizeof(struct St_NSP_PDCSMCS_SectionIni));
            if (sscanf(pcBuf, "[%[^]]", pstItem->acSectionName) != 1)
            {
                printf("无法解析 %s %d行\n", pfileName, i);
                exit(0);
            };
            pstItem->pstKeyValue = NULL;

            pstItem->pstNextSection = pstHead;
            pstHead = pstItem;
            // printf("[ %s ]\n",pstItem->acSectionName);
        }
        else
        {
            continue;
        }
    }

    free(pcBuf);
    return pstHead;
}

/*
 * 功能：获取节内的配置项的值
 * 参数：
 *		@pstItem[in]:指定的节
 *		@pcKey[in]:待获取配置项的键值
 * 返回值：
 *		-D_NSP_PDCSMCS_TICS_PARAM_NULL_ERR: 传入参数为空
 *		-D_NSP_PDCSMCS_TICS_GET_CONF_ITEM_ERR: 获取节内的配置项的值错误
 *		iTemp[out]:成功
 **/
int CSqlGradOperate::GetItemInt(St_NSP_PDCSMCS_SectionIni *pstItem, char *pcKey)
{
    int iTemp;
    if (pstItem == NULL)
    {
        return -D_NSP_PDCSMCS_TICS_PARAM_NULL_ERR;
    }

    struct Key_Value *pstKeyValue = pstItem->pstKeyValue;
    while (pstKeyValue != NULL && strcmp(pstKeyValue->acKey, pcKey))
    {
        pstKeyValue = pstKeyValue->pstNextKeyValue;
    }

    if (pstKeyValue != NULL)
    {
        sscanf(pstKeyValue->acValue, "%d", &iTemp);
        // printf("%d\n",iTemp);
        return iTemp;
    }

    return -D_NSP_PDCSMCS_TICS_GET_CONF_ITEM_ERR;
}

/*
 * 功能：获取节内的配置项的值
 * 参数：
 *		@pstItem[in]:指定的节
 *		@pcKey[in]:待获取配置项的键值
 * 返回值：
 *		NULL:传入参数为空
 *		@pstKeyValue[out]:成功
 **/
char *CSqlGradOperate::GetItemString(St_NSP_PDCSMCS_SectionIni *pstItem, char *pcKey)
{
    if (pstItem == NULL)
    {
        return NULL;
    }

    struct Key_Value *pstKeyValue = pstItem->pstKeyValue;
    while (pstKeyValue != NULL && strcmp(pstKeyValue->acKey, pcKey))
    {
        pstKeyValue = pstKeyValue->pstNextKeyValue;
    }

    if (pstKeyValue != NULL)
    {
        return pstKeyValue->acValue;
    }

    return NULL;
}

int CSqlGradOperate::SetBackUpDB()
{
    int ret = 0;
    DMStmt stDmStmt;
    // 1.将数据库设置为挂起状态
    std::string strSetMount = "alter database mount;";
    ret = BindSql2Exe(stDmStmt, strSetMount.c_str());
    if (ret < 0 && ret != -510)
    {
        LOG(LOG_ERROR, "将数据库设置为挂起状态 失败");
        return ret;
    }

    // 2.开启数据库的归档配置
    std::string strArchivelog = "alter database archivelog;";
    ret = BindSql2Exe(stDmStmt, strArchivelog.c_str());
    if (ret < 0)
    {
        LOG(LOG_ERROR, "开启数据库的归档配置 失败");
        return ret;
    }

    // 3.配置数据库的归档参数
    std::string strPath = m_systemIni.GetValue("dbInfo", "BackUpLogDir", "/dm8/backupUnExcept");
    std::string strLogConfig = "alter database add archivelog 'type=local,dest=" + strPath + ",file_size=1024,space_limit=4096';";
    ret = BindSql2Exe(stDmStmt, strArchivelog.c_str());
    if (ret < 0)
    {
        LOG(LOG_ERROR, "配置数据库的归档参数 失败");
        return ret;
    }

    // 4.修改回open状态
    std::string strSetOpen = "alter database open;";
    ret = BindSql2Exe(stDmStmt, strSetOpen.c_str());
    if (ret < 0 && ret != -514)
    {
        LOG(LOG_ERROR, "修改回open状态 失败");
        return ret;
    }

    // 5.初始化作业环境
    strSetOpen = "SP_INIT_JOB_SYS(1)";
    ret = BindSql2Exe(stDmStmt, strSetOpen.c_str());
    if (ret < 0 && ret != -806)
    {
        LOG(LOG_ERROR, "初始化作业环境 失败");
        return ret;
    }

    // 6.开启备份作业，设置备份策略：总备份和差分备份，我先设置一下总备份

    return 0;
}

/// @brief 数据库设置了每周一次的全备份
/// @return
int CSqlGradOperate::SetCompliBackUp()
{
    int ret = 0;
    bool flag = false;
    DMStmt stDmStmt;
    std::string strPath = m_systemIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");

    std::string sql = "call SP_CREATE_JOB('BackUp_center',1,0,'',0,0,'',0,'');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0 && ret != -8415)
    {
        if (ret == -8415)
        {
            LOG(LOG_DEBUG, "作业已经存在现在是更新作业");
            return UpdateCompliBackUp();
        }
        return ret;
    }
    sql = "call SP_JOB_CONFIG_START('BackUp_center');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_STEP('BackUp_center', 'BackUp_dm', 6, '00000000/" + strPath + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_SCHEDULE('BackUp_center', 'test1', 1, 2, 1, 64, 0, '00:00:00', NULL, '2024-01-02 23:48:24', NULL, '');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_JOB_CONFIG_COMMIT('BackUp_center');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
    return SUCCESS;
}

/// @brief 给数据库设置每天一次的增量备份
/// @return
int CSqlGradOperate::SetAddBackUp()
{
    int ret = 0;
    DMStmt stDmStmt;
    std::string strPathAdd = m_systemIni.GetValue("dbInfo", "BackUpAddDir", "/dm8/backup");
    std::string strPath = m_systemIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");

    std::string sql = "call SP_CREATE_JOB('BackUp_Add',1,0,'',0,0,'',0,'');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        if (ret == -8413)
        {
            // 已经有了这个全备份了，所以直接退出
            return UpdateAddBackUp();
        }
        return ret;
    }

    sql = "call SP_JOB_CONFIG_START('BackUp_Add');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_STEP('BackUp_Add', 'Add_test_buzhou', 6, '10000000" + strPath + "|" + strPathAdd + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ADD_JOB_SCHEDULE('BackUp_center', 'test1', 1, 2, 1, 64, 0, '00:00:00', NULL, '2024-01-02 23:48:24', NULL, '');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_JOB_CONFIG_COMMIT('BackUp_center');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
}

int CSqlGradOperate::UpdateCompliBackUp()
{
    int ret = 0;
    DMStmt stDmStmt;
    std::string strPath = m_systemIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");
    return SUCCESS;

    std::string sql = "call SP_JOB_CONFIG_START('BackUp_center');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_STEP('BackUp_center', 'BackUp_dm', 6, '00000000/" + strPath + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
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
    std::string strPathAdd = m_systemIni.GetValue("dbInfo", "BackUpAddDir", "/dm8/backup");
    std::string strPath = m_systemIni.GetValue("dbInfo", "BackUpDir", "/dm8/backup");
    return SUCCESS;

    std::string sql = "call SP_JOB_CONFIG_START('BackUp_Add');";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }

    sql = "call SP_ALTER_JOB_STEP('BackUp_Add', 'Add_test_buzhou', 6, '10000000" + strPath + "|" + strPathAdd + "', 0, 0, 0, 0, NULL, 0);";
    ret = BindSql2Exe(stDmStmt, sql.c_str());
    if (ret < 0)
    {
        return ret;
    }
    return SUCCESS;
}
