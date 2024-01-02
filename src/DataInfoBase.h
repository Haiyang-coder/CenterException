#pragma once
#include<string>

class DataInfoBase
{
private:
    /* data */
public:
    DataInfoBase(/* args */);
    virtual ~DataInfoBase();
    //展示说有现存的数据
    virtual void DisplayData() = 0;
    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString) = 0;
    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const = 0;
};
