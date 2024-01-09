#ifndef DELDUPCOMPLE_H
#define DELDUPCOMPLE_H

#include <string>
#include "DataInfoBase.h"

class DelDupComple : public DataInfoBase
{
public:
    DelDupComple();

    // Method for displaying data
    void DisplayData();

    // Method for converting from JSON string to object
    bool TurnStr2Obj(const char *jsonString);

    // Method for generating SQL statement for database insertion
    void GetInserDataInOrder(std::string &strData) const;

private:
    // Internal method for assigning JSON values to object members
    void assignValues(const Json::Value &root);

private:
    // Internal struct representing the content field
    struct Content
    {
        std::string infoID;
        std::string deleteDupFailSet;
        std::string deleteDupSuccSet;
        std::string deleteDupCompletenessDiff;
    };

    // Internal struct for holding data
    struct Data
    {
        unsigned int DataType;
        Content content;
    };

    // Internal struct object
    Data data;
};

#endif // DELDUPCOMPLE_H
