#pragma once
/*本类是要查询异常操作数据的请求格式*/

#include <string>
class CDataRequest
{
public:
    CDataRequest();
    ~CDataRequest();

public:
    short getType;            // 用什么方式获取数据
    short tableVersion;       // 想要什么异常
    std::string subtimeStart; // 开始时间
    std::string subtimeEnd;   // 结束时间
    long timeSpan;            // 间隔时间
};
