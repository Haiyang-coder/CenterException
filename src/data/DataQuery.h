#pragma once
/**本类是，其他模块发送来的，查询异常数据的类，根据他要查询的异常数据类型代号，异常数据的提交时间范围来锁定要用的数据*/

#include <string>
#include "DataInfoBase.h"
class CDataQuery : public DataInfoBase
{
public:
    CDataQuery(const char *jsonString);
    ~CDataQuery();

public:
    // 显示数据的函数
    virtual void DisplayData();
    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString);
    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const;

public:
    short getType;            // 用什么方式获取数据，0:tcp发包 1：在ftp服务器自己取
    short tableVersion;       // 想要什么异常
    std::string subtimeStart; // 开始时间
    std::string subtimeEnd;   // 结束时间
};
