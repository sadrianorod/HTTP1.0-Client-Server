//
// Created by lucas on 08/10/2020.
//
#include <string>

#ifndef LAB2CES35_REQHEADER_H
#define LAB2CES35_REQHEADER_H

enum ReqMethod {GET, HEAD, POST, UNSUPPORTED};
enum ReqType {SIMPLE, FULL};

class ReqHeader
{
public:
    int parseHttpHeader(std::string & buffer);
    int getRequest(int conn);
    void cleanUrl();
    ReqHeader();
    int status;
    enum ReqType type;

private:
    enum ReqMethod method;
    std::string referer;
    std::string userAgent;
    std::string resource;

};

#endif //LAB2CES35_REQHEADER_H
