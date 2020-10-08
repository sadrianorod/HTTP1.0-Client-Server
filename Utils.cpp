//
// Created by lucas on 08/10/2020.
//
#include "bits/stdc++.h"
#include <unistd.h>

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
            exit(0);
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
