//
// Created by lucas on 09/10/2020.
//

#include "Resource.h"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "Utils.h"
#include <fstream>


bool fexists(const char *filename) {
  std::ifstream ifile(filename);
  return (bool)ifile;
}

Resource::Resource(std::string path) {
    fileRoot = path;
}

int Resource::returnResource(int conn, int resource) {
    char c;
    int i;

    while((i = read(resource, &c, 1)))
    {
        if(i<0)
        {
            std::cerr << "Error reading file\n";
            exit(1);
        }

        if(write(conn, &c, 1) < 1)
        {
            std::cerr << "Error sending file\n";
            exit(1);
        }
    }

    return 0;
}

int Resource::checkResource(ReqHeader &info) {
    info.cleanUrl();
    if (strcmp(fileRoot.data(),".") == 0){
        fileRoot = "index.html";
    }
    fileRoot = "../Content/" + fileRoot;
    info.resource = fileRoot + info.resource;
    return open(fileRoot.data(), O_RDONLY);
}

void Resource::returnErrorMsg(int conn, ReqHeader &info) {
    std::string line = "<HTML>\n<HEAD>\n<TITLE>Server Error" + std::to_string(info.status) + "</TITLE>\n<HEAD>\n\n" +
            "<BODY>\n<h1>"  + std::to_string(info.status) + " " + getStatusMessage(info.status) + "</h1>\n"
            + "<p>The request did not work properly.</p>\n</BODY>\n</HTML>\n";
    writeLine(conn, line);
}