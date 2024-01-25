#pragma once
#include <string>
#include <vector>

class CDbRowData
{
public:
    CDbRowData(int data)
    {
        piData = data;
        strData = "";
    }
    CDbRowData(std::string data)
    {
        strData = data;
        piData = -1;
    }
    ~CDbRowData()
    {
    }

public:
    int piData;          // 整型内容
    std::string strData; // 字符串内容
};

class CDbData
{

public:
    CDbData(/* args */) {}
    ~CDbData() {}

public:
    int iLineSize;                       // 总行数
    int iClomuSize;                      // 总列数
    std::vector<std::string> vecCloName; // 所有列名
    std::vector<int> vecType;            // 每一列的数据类型 ： 2：整形 4：字符串
    std::vector<CDbRowData> vecDbRow;    // 所有查询到的数据
};
