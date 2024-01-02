
#include "DMsql.h"
 

#define DPIRETURN_CHECK(rt,hndl_type,hndl) if(!DSQL_SUCCEEDED(rt)){dpi_err_msg_print(hndl_type,hndl);return rt;}
/*********************************************************************
* 功能:
*		打印错误信息
* 参数:
*		@hndlType: 句柄类型
*		@hndl: 句柄
* 返回:
* 		打印错误信息
**********************************************************************/
void dpi_err_msg_print(sdint2 hndlType, dhandle hndl)
{
	sdint4 errCode; //错误代码
	sdint2 msgLen;  //消息长度
	sdbyte errMsg[SDBYTE_MAX]; //错误消息

	//获取错误信息集合，并打印 
	dpi_get_diag_rec(hndlType, hndl, 1, &errCode, errMsg, sizeof(errMsg), &msgLen);
	printf("error msg = %s,	error code = %d\n", errMsg, errCode);	
	
}


/*************************************************************************
* 功能:
*		连接数据库
* 参数:
*		@dmconn: 数据库连接结构体指针
*		@server: 服务器名称（IP地址）
*		@uer: 数据库登录帐号
* 		@password: 数据库登录密码
*		@uiPort: 登录端口
* 返回:
* 		DSQL_SUCCESS: 执行成功
*		DSQL_SUCCESS_WITH_INFO: 执行成功，有警告消息
*		DSQL_ERROR: 执行失败
*		DSQL_INVALID_HANDLE: 无效句柄
*************************************************************************/
int dm_connect(struct DMconn *dmconn, const char *server, const char *user, const char *password, long long llPort)
{  
	DPIRETURN rt; //函数返回值	

	//申请环境句柄
	rt = dpi_alloc_env(&(dmconn->henv));
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print( DSQL_HANDLE_ENV, dmconn->henv);
		return rt;
	}

	//申请连接句柄
	rt = dpi_alloc_con(dmconn->henv, &(dmconn->hcon));
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_DBC, dmconn->hcon);
		return rt;
	}
	dmconn->conFlag = 0; //连接标志初始化为0
  	
	//设置句柄属性，此处设置登录端口

	if(llPort == 0) 
	{
		rt = dpi_set_con_attr(dmconn->hcon, DSQL_ATTR_LOGIN_PORT, (dpointer)LOGINPORT, sizeof(LOGINPORT)); //使用默认端口
		if(!DSQL_SUCCEEDED(rt))	 
		{	
			dpi_err_msg_print(DSQL_HANDLE_DBC, dmconn->hcon);
			return rt;
		}
	}
	else
	{
		rt = dpi_set_con_attr(dmconn->hcon, DSQL_ATTR_LOGIN_PORT, (dpointer)llPort, sizeof(llPort));  //使用端口参数	//lzf modify, port改为传指针，20180803
		if(!DSQL_SUCCEEDED(rt))	 
		{	
			dpi_err_msg_print(DSQL_HANDLE_DBC, dmconn->hcon);
			return rt;
		}
	}	

	
	//连接数据库服务器
	rt = dpi_login(dmconn->hcon, (sdbyte *)server, (sdbyte *)user, (sdbyte *)password);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_DBC, dmconn->hcon);
		return rt;
	}
	dmconn->conFlag = 1;  //连接标志置 1


	//将结构体赋值给数据库句柄
	//*dmConn = (DMConnect)dmconn;  
		
	return DSQL_SUCCESS; 
}



/***************************************************************************
* 功能:
* 		申请语句句柄
* 参数:
*		@dmConn: 数据库连接结构体指针
*		@stDmStmt: 语句句柄
* 返回:
* 		DSQL_SUCCESS: 执行成功
*		DSQL_SUCCESS_WITH_INFO: 执行成功，有警告消息
*		DSQL_ERROR: 执行失败
*		DSQL_INVALID_HANDLE: 无效句柄
***************************************************************************/
int dm_alloc_stmt(struct DMconn *dmconn, DMStmt *stDmStmt)
{
	DPIRETURN rt; //函数返回值	
	//检查数据库句柄
	if(dmconn == NULL)
	{	
		printf("invalid database handle\n");
		return DSQL_INVALID_HANDLE;
	}		
	
	//没有连接到数据库
	if (dmconn->conFlag != 1)
	{
		printf("\nno connect to database!\n");
		return DSQL_INVALID_HANDLE;   
	}

	//申请语句句柄
	rt = dpi_alloc_stmt(dmconn->hcon, &(stDmStmt->hstmt));	
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
		return rt;
	}
	/*
	//设置游标属性
	ulength val = DSQL_CURSOR_DYNAMIC;  //动态游标，可以回滚
	rt = dpi_set_stmt_attr(stDmStmt->hstmt, DSQL_ATTR_CURSOR_TYPE, (dpointer)&val, 0);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
		return rt;
	}
	*/
	//结果集标志初始化
	stDmStmt->fieldFlag = 0; 
	stDmStmt->rowFlag = 0;

	return DSQL_SUCCESS; 
}

 

/***************************************************************************
* 功能:
* 		使用语句句柄DMStmt,执行SQL语句
* 参数:
*		@stDmStmt: 语句句柄
*		@buf: SQL语句
* 返回:
* 		DSQL_SUCCESS: 执行成功
*		DSQL_SUCCESS_WITH_INFO: 执行成功，有警告消息
*		DSQL_ERROR: 执行失败
*		DSQL_INVALID_HANDLE: 无效句柄
***************************************************************************/
int dm_query(DMStmt *stDmStmt, const char *buf)
{
	//如果传入的句柄为空
	if(stDmStmt == NULL)
		return DSQL_INVALID_HANDLE;  
	
	DPIRETURN rt; //函数返回值			
	
	//如果语句句柄已有结果集(标志>0)
	if ( stDmStmt->fieldFlag)
	{
		//printf("*****fieldFlag > 0, close the current cursor!*****\n");
		rt = dpi_close_cursor(stDmStmt->hstmt);  //关闭当前游标
		if(!DSQL_SUCCEEDED(rt))	 
		{	
			dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
			return rt;
		}
	}
	
	//执行SQL语句
	rt = dpi_exec_direct(stDmStmt->hstmt, (sdbyte *)buf);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
		return rt;
	}

	
	//统计结果集列数，
	stDmStmt->fieldFlag = 0;  //标志设为0，表示没有结果集
	rt = dpi_number_columns(stDmStmt->hstmt, &(stDmStmt->fieldFlag));  //结果集标志等于列数，如果列数大于0，则结果集标志大于0
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
		return rt;
	}
		
 		
	//统计受影响的行数
	stDmStmt->rowFlag = 0;  
	rt = dpi_row_count(stDmStmt->hstmt, &(stDmStmt->rowFlag));
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
		return rt;
	}

	return DSQL_SUCCESS;  
 
}
 


/***************************************************************************
* 功能:
* 		打印返回集结果
* 参数:
*		@dmConn: 数据库句柄
*		@row: 控制输出行数，一般等于结果集的行数
*		@result[][COLNUM][COLSIZE]: 返回结果数组
* 返回:
* 		DSQL_SUCCESS: 执行成功
*		DSQL_SUCCESS_WITH_INFO: 执行成功，有警告消息
*		DSQL_ERROR: 执行失败
*		DSQL_INVALID_HANDLE: 无效句柄
***************************************************************************/
int dm_query_result(DMStmt *stDmStmt, long long int row, short col, char result[][COLUMN][COLSIZE])
{
	//如果传入的句柄为空
	if(stDmStmt == NULL)
	{	
		printf("invalid handle\n");
		return DSQL_INVALID_HANDLE;
	}

	DPIRETURN rt; //函数返回值		

	//如果没有结果集(标志为0)
	if ( !stDmStmt->fieldFlag)
	{
		printf("fieldFlag = 0, no return result!\n");
		return DSQL_INVALID_HANDLE;   
	}

	//如果结果集为空
	if ( !stDmStmt->rowFlag) 
	{ 
		printf("rowFlag = 0 ,no matched data!\n");
		return DSQL_NO_DATA;
	}

	//列参数
	ulength colSize;   //列实际宽度
	sdint2 icol;  //列索引号

	//结果集参数
	ulength rowNum;  //游标所在行号 
	sdint8 iRowCount;  //记录所在行号

	//申请绑定列的缓冲区
	sdbyte (*colName)[COLSIZE] = (sdbyte(*)[COLSIZE])malloc((stDmStmt->fieldFlag) * COLSIZE * sizeof(sdbyte)); 
	if(colName == NULL)
	{
		printf("alloc colName memory failed!\n");
		return DSQL_ERROR;
	}

	//绑定列
	for(icol = 1; icol <= (stDmStmt->fieldFlag); icol++)
	{
/*		//获取列的描述，colName[]中存放列的名称，暂不需要
		rt=dpi_desc_column(dmconn->hstmt, icol, colName[icol - 1], sizeof(colName[icol - 1]), &colNameLen, &sqltype, &colSize, &colDigits, &nullable);
		if(!DSQL_SUCCEEDED(rt))	 
		{	
			dpi_err_msg_print(DSQL_HANDLE_STMT, dmconn->hstmt);
			return rt;
		}
*/
		//绑定列  	
		rt=dpi_bind_col(stDmStmt->hstmt, icol, DSQL_C_NCHAR, colName[icol - 1], sizeof(colName[icol - 1]), (slength *)&colSize);  //结果统一转成DSQL_C_NCHAR格式
		if(!DSQL_SUCCEEDED(rt))	 
		{	
			dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
			return rt;
		}
	}
		
	//打印结果集
	iRowCount = 0; 
	while((iRowCount < row) && (dpi_fetch_scroll(stDmStmt->hstmt, DSQL_FETCH_NEXT, 0, &rowNum) != DSQL_NO_DATA) ) 
	{
		for(icol = 0; (icol < col) && (icol < (stDmStmt->fieldFlag)); icol++)
		{	
			strcpy((char*)result[iRowCount][icol], (char*)colName[icol]);	//从第0行开始存放
		} 

		iRowCount++;
	}


	//释放动态数组
	free(colName);

	return DSQL_SUCCESS;           	
}
 


/***************************************************************************
* 功能:
* 		释放语句句柄
* 参数:
*		@stDmStmt: 语句句柄
* 返回:
* 		DSQL_SUCCESS: 执行成功
*		DSQL_SUCCESS_WITH_INFO: 执行成功，有警告消息
*		DSQL_ERROR: 执行失败
*		DSQL_INVALID_HANDLE: 无效句柄
***************************************************************************/
int dm_free_stmt(DMStmt *stDmStmt)
{
	//如果传入的句柄为空
	if(stDmStmt == NULL)
	{
		return DSQL_INVALID_HANDLE;
	}

	DPIRETURN rt; //函数返回值

	//释放语句句柄
	rt = dpi_free_stmt(stDmStmt->hstmt);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_STMT, stDmStmt->hstmt);
		return rt;
	}

	//释放结构体
	//free(stDmStmt);  //不需要

	return DSQL_SUCCESS;  

}

/***************************************************************************
* 功能:
*		断开连接数据库
* 参数:
*		@dmConn: 数据库连接结构体指针
* 返回:
* 		DSQL_SUCCESS: 执行成功
*		DSQL_SUCCESS_WITH_INFO: 执行成功，有警告消息
*		DSQL_ERROR: 执行失败
*		DSQL_INVALID_HANDLE: 无效句柄
***************************************************************************/
int F_NSP_PDCSMCS_dm_close(struct DMconn *dmconn)
{	
	DPIRETURN rt; //函数返回值
	//如果传入的句柄为空
	if(dmconn == NULL)
	{
		return DSQL_INVALID_HANDLE;
	}

	//没有连接到数据库
	if (dmconn->conFlag != 1)
	{
		printf("\nno connect to database!\n");
		return DSQL_INVALID_HANDLE;   
	}

	//退出登录，断开连接
	if(dmconn->hcon == NULL)
	{
		printf("dmconn->hcon NULL");
		return DSQL_INVALID_HANDLE;
	}		
	rt = dpi_logout(dmconn->hcon);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_DBC, dmconn->hcon);
		return rt;
	}
	dmconn->conFlag = 0;  //连接标志置 0

	//释放连接句柄
	rt = dpi_free_con(dmconn->hcon);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_DBC, dmconn->hcon);
		return rt;
	}

	//释放环境句柄	
	if(dmconn->henv == NULL)
	{
		printf("dmconn->henv NULL");
		return DSQL_INVALID_HANDLE;
	}	
	rt = dpi_free_env(dmconn->henv);
	if(!DSQL_SUCCEEDED(rt))	 
	{	
		dpi_err_msg_print(DSQL_HANDLE_ENV, dmconn->henv);
		return rt;
	}

	return DSQL_SUCCESS;

}


/************************************
 * 20141201 林佳楠
 * 直接执行SQL语句
 * **********************************/
DPIRETURN databaseExecuteDirect(DMStmt *stDmStmt, const char* sqlStr)
{
	DPIRETURN rt; //函数返回值
	/*
    if (conFlag != 1)
    {
        this->databaseConnect((sdbyte *)DM_SVR, (sdbyte *)DM_USER, (sdbyte *)DM_PWD, DM_PORT);
        if (conFlag != 1)
            return DSQL_ERROR;
    }
	*/

    //执行SQL
    rt = dpi_exec_direct(stDmStmt->hstmt, (sdbyte *)sqlStr);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return rt;
}

/************************************
 * 20141201 林佳楠
 * 统计结果集行数（需先执行select count语句）
 * **********************************/
int databaseRowCountSelect(DMStmt *stDmStmt)
{
	DPIRETURN rt; //函数返回值
    int iRowCount;
    slength rowCountInd;
    rt = dpi_bind_col(stDmStmt->hstmt, 1, DSQL_C_SLONG, &iRowCount, sizeof(iRowCount), &rowCountInd);
    dpi_fetch(stDmStmt->hstmt, NULL);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return iRowCount;
}

/************************************
 * 20141201 林佳楠
 * 绑定结果集的一列
 * **********************************/
DPIRETURN databaseBindColumn(DMStmt *stDmStmt, unsigned short column, short type, void* c, long long bufLength, long long *ind)
{
	DPIRETURN rt; //函数返回值
    //绑定一列
    rt = dpi_bind_col(stDmStmt->hstmt, (udint2)column, (sdint2)type, (dpointer)c, (slength)bufLength, (slength *)ind);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return rt;
}

/************************************
 * 获取结果集的一行
 * **********************************/
DPIRETURN databaseFetch(DMStmt *stDmStmt)
{
	DPIRETURN rt; //函数返回值
    ulength row_num;
    //获取一行
    rt = dpi_fetch(stDmStmt->hstmt, &row_num);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return rt;
}

/************************************
 * 统计SQL受影响的行数（需先执行update, insert和delete语句）
 * **********************************/
int databaseRowCount(DMStmt *stDmStmt)
{
	DPIRETURN rt; //函数返回值
    sdint8 iRowCount;
    rt = dpi_row_count(stDmStmt->hstmt, &iRowCount);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return (int)iRowCount;
}

DPIRETURN databasePrepare(DMStmt *stDmStmt, const char *sqlStr)
{
	DPIRETURN rt; //函数返回值
    rt = dpi_prepare(stDmStmt->hstmt, (sdbyte *)sqlStr);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return rt;
}

DPIRETURN databaseBindParam(DMStmt *stDmStmt, unsigned short iparam, unsigned short param_type, unsigned short ctype,
                                  unsigned short dtype, unsigned int precision, short scale,
                                  void* buf, long long buf_len, long long *ind_ptr)
{
	DPIRETURN rt; //函数返回值
    rt = dpi_bind_param(stDmStmt->hstmt, (udint2)iparam, (sdint2)param_type, (sdint2)ctype, (sdint2)dtype, (ulength)precision, (sdint2)scale, (dpointer)buf, (slength)buf_len, (slength *)ind_ptr);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return rt;
}

DPIRETURN databaseExecute(DMStmt *stDmStmt)
{
	DPIRETURN rt; //函数返回值
    rt = dpi_exec(stDmStmt->hstmt);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    return rt;
}

DPIRETURN databaseWriteFileFromGetData(DMStmt *stDmStmt, FILE *file, unsigned short index)
{
    sdbyte tempBuffer[1024];
    slength length;
    if (file == NULL)
        return DSQL_ERROR;

    while (DSQL_SUCCEEDED(dpi_get_data(stDmStmt->hstmt, (udint2)index, DSQL_C_BINARY, tempBuffer, 1024, &length)))
    {
        if (length > 1024)
            length = 1024;
        fwrite(tempBuffer, sizeof(char), length, file);
    }
    return DSQL_SUCCESS;
}

DPIRETURN databaseUpdateStatementHandle(struct DMconn *dmconn, DMStmt *stDmStmt)
{
	DPIRETURN rt; //函数返回值
    //释放语句句柄
    rt = dpi_free_stmt(stDmStmt->hstmt);
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
    //申请语句句柄
    rt = dpi_alloc_stmt(dmconn->hcon, &(stDmStmt->hstmt));
    DPIRETURN_CHECK(rt, DSQL_HANDLE_STMT, stDmStmt->hstmt);
	return rt;
}


