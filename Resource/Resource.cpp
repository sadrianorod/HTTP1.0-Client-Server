//
// Created by lucas on 09/10/2020.
//

#include "Resource.h"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "Utils.h"
#include <fstream>
#include <cmath>


Resource::Resource(std::string path) {
    fileRoot = path;
}
 

int Resource::returnResource(int conn) {
    file.seekg(0, std::ios_base::end);
    std::size_t fileSize = file.tellg(); //Tamanho do arquivo em bytes!
    file.seekg(0, std::ios_base::beg);
    std::vector<char> buffer;
    buffer.reserve(fileSize);
    std::copy(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(),
                    std::back_inserter(buffer));
   
    std::size_t maxSize = 1 << 16;
    char * ptr = reinterpret_cast<char*>(buffer.data());
    std::size_t sent = 0;
    while(sent + maxSize < fileSize)
    {
        if(write(conn, ptr, maxSize) < 1)
        {
            std::cerr << "Error sending file\n";
            break;
        }
        sent += maxSize;
        ptr += maxSize;
    }

    if(sent < fileSize)
    {
        write(conn, ptr, fileSize - sent);
    }


    file.close();
    return 0;
}

std::size_t Resource::checkResource(ReqHeader &info) {
    info.cleanUrl();
    info.resource = fileRoot + info.resource;
    
    file.open(info.resource);
    if(file.is_open()){
       file.seekg(0, std::ios_base::end);
       std::size_t fileSize = file.tellg(); //Tamanho do arquivo em bytes!
       file.seekg(0, std::ios_base::beg);
       return fileSize;
    }

    file.open(info.resource + "/index.html");
    if(file.is_open()){
       file.seekg(0, std::ios_base::end);
       std::size_t fileSize = file.tellg(); //Tamanho do arquivo em bytes!
       file.seekg(0, std::ios_base::beg);
       return fileSize;                  
     }
    return 0;
}

void Resource::returnErrorMsg(int conn, ReqHeader &info) {
    std::string line = "<HTML>\n<HEAD>\n<TITLE>Server Error" + std::to_string(info.status) + "</TITLE>\n<HEAD>\n\n" +
            "<BODY>\n<h1>"  + std::to_string(info.status) + " " + getStatusMessage(info.status) + "</h1>\n"
            + "<p>The request did not work properly.</p>\n</BODY>\n</HTML>\n";
    writeLine(conn, line);
}
