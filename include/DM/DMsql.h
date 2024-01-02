#ifndef DMSQL2_H
#define DMSQL2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DPI.h"
#include "DPIext.h"
#include "DPItypes.h"

#define STR_LEN 1024	//读取配置文件和构造sql语句
#define COLUMN 20   //列数 
#define COLSIZE 200  //列宽度 ,注意内存限制
#define LOGINPORT 5236  //默认登录端口

/*达梦头文件宏定义,函数返回值
#define DSQL_SUCCESS	0
#define DSQL_SUCCESS_WITH_INFO	1
#define DSQL_NO_DATA	100
#define DSQL_ERROR	(-1)
#define DSQL_INVALID_HANDLE	(-2)
*/


/*DM连接句柄*/
typedef void* DMConnect;  //DMConnect和DMconn之间互换


/*DM连接结构体*/
typedef struct DMconn
{
	dhenv henv;	//环境句柄	
	dhcon hcon;  //连接句柄
	udbyte conFlag;	//连接状态

}DMconn;


/*DM语句句柄*/
typedef struct DMStmt
{
	dhstmt hstmt; //语句句柄
	sdint2 fieldFlag;//有无结果集的标志,如果有,等于结果集的列数
	sdint8 rowFlag;//受影响的行数，如果有结果集，等于结果集的行数 
}DMStmt;


/*连接数据库 */
int dm_connect(struct DMconn *dmHandle, const char *host, const char *user, const char *password, long long uiPort);


/*申请语句句柄*/
int dm_alloc_stmt(struct DMconn *dmconn, DMStmt *stDmStmt);


/*执行SQL语句*/
int dm_query(DMStmt *stDmStmt, const char *buf); 


/*打印结果集*/
int dm_query_result(DMStmt *stDmStmt, long long int row, short col, char result[][COLUMN][COLSIZE]);  //new


/*释放语句句柄*/
int dm_free_stmt(DMStmt *stDmStmt);


/*断开连接数据库*/
int F_NSP_PDCSMCS_dm_close(struct DMconn *dmconn);


/*打印错误信息*/ 
void dpi_err_msg_print(sdint2 hndlType, dhandle hndl);


DPIRETURN databaseExecuteDirect(DMStmt *stDmStmt, const char* sqlStr);

/************************************
 * 统计结果集行数（需先执行select count语句）
 * **********************************/
int databaseRowCountSelect(DMStmt *stDmStmt);

/************************************
 * 绑定结果集的一列
 * **********************************/
DPIRETURN databaseBindColumn(DMStmt *stDmStmt, unsigned short column, short type, void* c, long long bufLength, long long *ind);

/************************************
 * 获取结果集的一行
 * **********************************/
DPIRETURN databaseFetch(DMStmt *stDmStmt);

/************************************
 * 统计SQL受影响的行数（需先执行update, insert和delete语句）
 * **********************************/
int databaseRowCount(DMStmt *stDmStmt);

DPIRETURN databasePrepare(DMStmt *stDmStmt, const char *sqlStr);

DPIRETURN databaseBindParam(DMStmt *stDmStmt, unsigned short iparam, unsigned short param_type, unsigned short ctype,
                                  unsigned short dtype, unsigned int precision, short scale,
                                  void* buf, long long buf_len, long long *ind_ptr);

DPIRETURN databaseExecute(DMStmt *stDmStmt);

DPIRETURN databaseWriteFileFromGetData(DMStmt *stDmStmt, FILE *file, unsigned short index);

DPIRETURN databaseUpdateStatementHandle(struct DMconn *dmconn, DMStmt *stDmStmt);


#endif // DMSQL2_H
