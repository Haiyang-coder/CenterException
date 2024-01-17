#ifndef CMDRESULT_H
#define CMDRESULT_H
#define SUCCESS 0
#define FAILED -1

// 总配置文件路径
#define SYSTEM_INI_FILE_PATH "/root/CenterExceptional/config/system_info.ini"

// conmmon use
#define MAX_SIZE 1024

// 数据库数据类型
#define TYPE_INT 2
#define TYPE_STRING 4

//
/***********************读取数据库需要的宏***************************************/

#define D_NSP_PDCSMCS_TICS_BUF_LEN 64				   //
#define D_NSP_PDCSMCS_TICS_DB_VALUE_LEN 1024		   // 键值长度
#define D_NSP_PDCSMCS_TICS_TIMESTAMP_LEN 22			   // 时间戳字符长度
#define D_NSP_PDCSMCS_TICS_CLOMN_NAME_LEN 32		   // 数据库列名长度
#define D_NSP_PDCSMCS_TICS_TOKEN_NUMBER 100			   // TOKEN NUMBER
#define D_NSP_PDCSMCS_TICS_TABLE_COL_LEN 16			   //
#define D_NSP_PDCSMCS_TICS_COLLECT_AGT_ID_LEN 8		   // 代理ID长度
#define D_NSP_PDCSMCS_TICS_STATE_TYPE_LEN 4			   // 状态类型长度
#define D_NSP_PDCSMCS_TICS_CPU_USAGE_LEN 8			   // cpu利用率长度
#define D_NSP_PDCSMCS_TICS_STORE_USAGE_LEN 8		   // 内存所占长度
#define D_NSP_PDCSMCS_TICS_NET_USAGE_LEN 8			   // 网络利用长度
#define D_NSP_PDCSMCS_TICS_MAX_LINE_LEN 1024		   // 一行字符串最大长度
#define D_NSP_PDCSMCS_TICS_QUERY_LINE_LEN 2048		   // 数据库查询语句长度
#define D_NSP_PDCSMCS_TICS_INSERT_LINE_LEN 1024 * 1024 // 数据库插入语句长度
#define D_NSP_PDCSMCS_TICS_MAX_JSON_LEN 1024 * 1024	   // JSON 最大数据长度

const int g_read_line_maxsize = 1024;

/***********************数据解析相关失败***************************************/
#define D_NSP_PDCSMCS_TICS_RECV_DATA_QUEUE_FAIL_ERR 0x03000001 // 接收数据任务队列初始化失败
#define D_NSP_PDCSMCS_TICS_QUEUE_NULL_ERR 0x03000002		   // 任务队列为空
#define D_NSP_PDCSMCS_TICS_JSON_PARSE_ERR 0x03000003		   // json数据解析失败
#define D_NSP_PDCSMCS_TICS_PARSE_SYSTEM_LOG_ERR 0x03000004	   // 解析系统日志信息错误
#define D_NSP_PDCSMCS_TICS_PARAM_NULL_ERR 0x03000005		   // 传入参数为空
#define D_NSP_PDCSMCS_TICS_GET_CONF_ITEM_ERR 0x03000006		   // 获取节内的配置项的值错误
#define D_NSP_PDCSMCS_TICS_NULL_POINTER_ERR 0x03000007		   // 空指针
#define D_NSP_PDCSMCS_TICS_ENCRYPT_FAILED_ERR 0x03000008	   // 加密失败错误码
#define D_NSP_PDCSMCS_TICS_DECRYPT_FAILED_ERR 0x03000009	   // 解密失败错误码

// 数据库操作错误码
#define SH_LOD_SUCCESS 0x00 // success
#define SH_SQL_GETHANDLE_ERROR 0x01
#define SH_SQL_BINDDATA_ERROR 0x02
#define SH_SQL_EXECUTESQL_ERROR 0x03
#define SH_SQL_BINDCLOMN_ERROR 0x04
#define SH_SQL_NO_DATA 0x05

/**********************通用型错误码*******************************************/
#define D_NSP_TDYTH_ERR_OK 0x00000000				   // 正确
#define D_NSP_TDYTH_RETURN_FAIL -1					   // 通用型错误
#define D_NSP_TDYTH_CONFIG_ERR 0x00000001			   // 配置文件出错
#define D_NSP_TDYTH_ERR_INVALID_PARAMETER 0x00000005   // 无效的参数输入
#define D_NSP_TDYTH_ERR_MALLOC_FIAL 0x0000000F		   // 动态申请空间失败
#define D_NSP_TDYTH_ERR_VERSION_NOT_FIND 0x00010001	   // 版本类型错误
#define D_NSP_TDYTH_ERR_CRYPT_MODE_NOT_FIND 0x00010002 // 加密类型错误
#define D_NSP_TDYTH_ERR_MAC_MODE_NOT_FIND 0x00010003   // MAC类型错误
#define D_NSP_TDYTH_ERR_LEN_CHECK_ERROR 0x00010004	   // 长度计算错误
#define D_NSP_TDYTH_ERR_PROTO_BUF_TOO_SHORT 0x00000010 // 输出数据缓冲区长度短
#define D_NSP_TDYTH_ERR_SYSTEMID 0x00010011			   // 系统ID错误

/**********************文件操作错误码*******************************************/
#define D_NSP_TDYTH_ERR_FILE_OPEN_ERROR 0x00020001	 // 文件打开错误
#define D_NSP_TDYTH_ERR_FILE_READ_ERROR 0X00020007	 // 文件读错误
#define D_NSP_TDYTH_ERR_FILE_MODIFY_ERROR 0X00020008 // 文件修改失败
#define D_NSP_TDYTH_ERR_FILE_CREATE_ERROR 0x00020009 // 文件创建失败

/**********************JSON相关错误码*******************************************/
#define D_NSP_TDYTH_ERR_PARSE_CJSON 0x00070001 // 解析json出错

/**********************通信失败**************************************/
#define D_NSP_TDYTH_ERR_COMMUNICATE_MSG_SEND 0x00060101	 // 消息队列发送数据失败
#define D_NSP_TDYTH_ERR_MSG_RECV_LEN_TOO_LONG 0x00060102 // 接收消息队列的数据过长
#define D_NSP_TDYTH_ERR_COMMUNICATE_MSG_GET 0x00060103	 // 消息队列发送数据失败

/**********************目的ID**************************************/
#define TASKID_QUERY_ALL_NEXTPAGE_DATA 0x00000001			// 查询数据库表下一页信息
#define TASKID_QUERY_ALL_PREPAGE_DATA 0x00000002			// 查询数据库表上一页信息
#define TASKID_QUERY_ALL_THISPAGE_DATA 0x00000003			// 查询数据库表本业信息
#define TASKID_QUERY_TABLENAME_DATA 0x00000004				// 查询数据库所有的表名
#define TASKID_QUERY_SEARCHINFO_BYIDANDTABLENAME 0x00000005 // 查根据id和表精确查询
#define TASKID_QUERY_DATAPATH 0x00000006					// 获取流转路径
#define TASKID_QUERY_SAVEFILE 0x00000006					// 将数据以文件形式下载

/**********************前后台通信相关**************************************/
#define D_NSP_IOTC_USMRS_MAX_PKT_LEN 65536 // 每一个数据包最大长度

/***********************数据库操作失败***************************************/
#define D_NSP_PDCSMCS_TICS_ALLOC_ERR 0x00030001				 // 获取语句句柄错误
#define D_NSP_PDCSMCS_TICS_PREPARE_ERR 0x00030002			 // 数据参数准备错误
#define D_NSP_PDCSMCS_TICS_BIND_PARAM_ERR 0x00030003		 // 绑定参数错误
#define D_NSP_PDCSMCS_TICS_INSERT_ERR 0x00030004			 // 执行插入错误
#define D_NSP_PDCSMCS_TICS_DATABASE_FREE_FAIL_ERR 0x00030005 // 数据库释放失败
#define D_NSP_PDCSMCS_TICS_DB_EXE_ERR 0x00030006			 // 数据库执行失败

/**
 * 列数据的名称, 一个表中的列名和属性详细信息
 */

struct Data_Map
{
	unsigned short usType; // 配置文件对应的列类型索引【用于列邦定】
	unsigned short usTag;  // 数据被解析标记：若被解析则值为1，否则为0
	unsigned char aucColumnName[D_NSP_PDCSMCS_TICS_BUF_LEN];
	unsigned char aucType[D_NSP_PDCSMCS_TICS_BUF_LEN]; // 列属性
};

struct St_NSP_PDCSMCS_DataTable
{
	unsigned char aucTableName[D_NSP_PDCSMCS_TICS_BUF_LEN];
	unsigned short usSize;	  // 列数目，表的列数目
	struct Data_Map astMap[]; // 每一列的属性
};
#define DATATABLELEN sizeof(struct St_NSP_PDCSMCS_DataTable)
#define INNERDATAMAPLEN sizeof(struct Data_Map)

/**
 * 数据库的表
 */
struct St_NSP_PDCSMCS_TableMap
{
	unsigned char aucSchema[D_NSP_PDCSMCS_TICS_BUF_LEN];		// 模式名称
	unsigned char aucAuthorization[D_NSP_PDCSMCS_TICS_BUF_LEN]; // 模式的管理人员
	unsigned short usSize;										// 表数目
	struct St_NSP_PDCSMCS_DataTable *pstTable[];				// 表的指针
};
#define TABLEMAPLEN sizeof(struct St_NSP_PDCSMCS_TableMap)
#define POINTERLEN sizeof(void *)

/*
 * 数据库查询结果结构体
 * */

typedef struct
{
	unsigned short usType;	// 数据类型 2:整形 4:const char*
	int piData;				// 整型内容
	char aucData[MAX_SIZE]; // const char* 内容
} PersonnalDataLine;

/*
 * 读取数据库配置文件结构体
 *
 * */
struct Key_Value
{
	char acKey[D_NSP_PDCSMCS_TICS_BUF_LEN];
	char acValue[D_NSP_PDCSMCS_TICS_BUF_LEN];
	struct Key_Value *pstNextKeyValue;
};

struct St_NSP_PDCSMCS_SectionIni
{
	char acSectionName[D_NSP_PDCSMCS_TICS_BUF_LEN];
	struct Key_Value *pstKeyValue;
	struct St_NSP_PDCSMCS_SectionIni *pstNextSection;
};

#endif // CMDRESULT_H
