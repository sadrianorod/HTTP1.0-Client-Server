//
// Created by lucas on 08/10/2020.
//
#include <unistd.h>
#include "Utils.h"


std::size_t readLine(int socket, std::string & buffer, std::size_t maxLen)
{
    std::size_t n, rc;
    char c;
    for(n = 1; n <maxLen; n++)
    {
        if((rc = read(socket, &c, 1)) == 1)
        {
            buffer+= c;
            if(c == '\n')
                break;
        }
        else if(rc == 0)
        {
            if(n == 1)
                return 0;
            break;
        }
        else
        {
            if(errno == EINTR)
                continue;
            std::cerr << "Error reading line\n";
            exit(1);
        }
    }

    return n;
}

std::string trim(const std::string & str)
{
    int size = str.size();
    while(!isalnum(str[size-1]) && size > 1)
        size --;

    return str.substr(0, size);
}

void writeLine(int socket, std::string & str)
{
    std::size_t left = str.size();
    std::size_t written;

    char * buffer = new char[left];
    strcpy(buffer, str.c_str());
    const char * ctePtr = buffer;

    while (left > 0)
    {
        if((written = write(socket, ctePtr, left)) <= 0)
        {
            if(errno == EINTR)
                written = 0;
            else
            {
                std::cerr << "Error writing line to socket\n";
                exit(1);
            }
        }
        left -= written;
        ctePtr += written;
    }

    delete[] buffer;
}

void outputHttpHeaders(int conn, int status)
{
    std::string line = "HTTP/1.0 " + std::to_string(status) + " OK\r\n"
            + "Server: Tanaka's server\r\n"
            + "Content-Type: text/html\r\n"
            + "\r\n";
    writeLine(conn, line);
}