#include "DataInfoBase.h"

DataInfoBase::DataInfoBase()
    : systemID(0), systemIP(""), mainCMD(0), subCMD(0), evidenceID(""), msgVersion(0),
      submittime(""), dataHash(""), datasign(""), randomidentification("")
{
}

DataInfoBase::~DataInfoBase()
{
}
