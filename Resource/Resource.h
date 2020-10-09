//
// Created by lucas on 09/10/2020.
//

#include <string>
#include "reqHeader.h"

#ifndef LAB2CES35_RESOURCE_H
#define LAB2CES35_RESOURCE_H

class Resource
{
public:
    int returnResource(int conn, int resource);
    int checkResource(ReqHeader & info);
    void returnErrorMsg(int conn, ReqHeader & info);
    explicit Resource(std::string path);

private:
    std::string fileRoot;
};

#endif //LAB2CES35_RESOURCE_H
