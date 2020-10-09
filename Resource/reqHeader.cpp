//
// Created by lucas on 08/10/2020.
//

#include "reqHeader.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "Utils.h"

ReqHeader::ReqHeader() : method(UNSUPPORTED), status(200) {}

int ReqHeader::parseHttpHeader(std::string & buffer) {
    static int firstHeader = 1;

    if(firstHeader == 1) {
        if (buffer.substr(0, 4) == "GET ") {
            method = GET;
            buffer.erase(0, 4);
        } else if (buffer.substr(0, 5) == "HEAD ") {
            method = HEAD;
            buffer.erase(0, 5);
        } else {
            method = UNSUPPORTED;
            status = 501;
            return -1;
        }

        int size = buffer.size();
        int i = 0;
        while (i < size && isspace(buffer[i]))
            i++;

        int j = i;
        while (j < size && buffer[j] != ' ')
            j++;

        if (j == i) {
            status = 400;
            return -1;
        }

        resource = buffer.substr(i, j - i);
        buffer.erase(0, i);
        if (buffer.find("HTTP/") != std::string::npos) {
            type = FULL;
        }
        else
            type = SIMPLE;

        firstHeader = 0;
        return 0;
    }

    int i=0;
    int size = buffer.size();
    while(i < size && buffer[i] != ':')
        i++;
    if(i == size)
    {
        status = 400;
        return -1;
    }

    std::string temp = buffer.substr(0, i);
    std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    buffer.erase(0, i+1);
    i=0;
    size = buffer.size();
    while(i < size && isspace(buffer[i]))
        i++;
    if(i == size)
        return 0;

    buffer.erase(0, i);
    if(temp == "USER-AGENT" || temp == "REFERER")
    {
        userAgent = buffer;
    }

    return 0;
}

int ReqHeader::getRequest(int conn) {
    std::string buffer = "";
    int rval;
    fd_set fds;
    timeval tv;

    //Timeout of 10 seconds
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    do {
        FD_ZERO(&fds);
        FD_SET(conn, &fds);

        rval = select(conn + 1, &fds, NULL, NULL, &tv);

        if(rval < 0) {
            std::cerr << "Error in select at getRequest\n";
            exit(1);
        }
        else if(rval == 0)
        {
            return -1;
        }
        else {
            //1024 is the maximum request line size
            readLine(conn, buffer, 1024);
            buffer = trim(buffer);

            if(buffer.size() == 0)
                break;

            if(parseHttpHeader(buffer))
                break;
        }
    } while (type != SIMPLE);

    return 0;
}

void ReqHeader::cleanUrl() {

    char num[3] = {0};
    int c;
    int size = resource.size();
    for(int i=0; i<size; i++)
    {
        if(resource[i] == '+')
            resource[i] = ' ';
        else if(resource[i] == '%')
        {
            num[0] = resource[i+1];
            num[1] = resource[i+2];
            resource[i] = strtol(num, NULL, 16);
            c = i+1;
            do {
                resource[c] = resource[c+2];
            } while (2 + c++ < size);
        }
    }
}