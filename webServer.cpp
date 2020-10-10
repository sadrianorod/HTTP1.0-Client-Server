#include <bits/stdc++.h>
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <netinet/in.h>
#include <unistd.h>
#include "Resource/reqHeader.h"
#include "Resource/Resource.h"
#include "Resource/Utils.h"

int serviceRequest(int connection, std::shared_ptr<Resource> & resourceManager)
{
    ReqHeader reqInfo;

    if(reqInfo.getRequest(connection) < 0)
        return -1;

    if(reqInfo.status == 200)
    {
        if(!resourceManager->checkResource(reqInfo))
        {
            reqInfo.status = 404;
        }
    }

    if(reqInfo.type == FULL)
    {
        outputHttpHeaders(connection, reqInfo.status);
    }

    if(reqInfo.status == 200)
    {
        if(resourceManager->returnResource(connection))
        {
            std::cerr << "Could not return resource\n";
            exit(1);
        }
    }
    else
        resourceManager->returnErrorMsg(connection, reqInfo);


    return 0;
}


void manageConnection(int connection, std::shared_ptr<Resource> resourceManager)
{

    serviceRequest(connection, resourceManager);

    if(close(connection) < 0)
    {
        std::cerr << "Error closing connection on thread\n";
        exit(1);
    }
}


int main(int argc, char*argv[])
{
    int sockListener;
    int connection;
    sockaddr_in servAddr{};
    if(argc < 4)
    {
        std::cerr << "Not enough arguments!\n";
        exit(1);
    }
    std::string hostName(argv[1]);
    std::string port(argv[2]);
    std::string directory(argv[3]);

    if((sockListener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Could no create sock listener\n";
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(std::stoi(port));

    if(bind(sockListener, (sockaddr * ) &servAddr, sizeof(servAddr)) < 0)
    {
        std::cerr << "Could not bind socket\n";
        exit(1);
    }

    if(listen(sockListener, 1024) < 0)
    {
        std::cerr << "Could not start listening to socket\n";
    }

    while(true)
    {
        if((connection = accept(sockListener, NULL, NULL)) < 0)
        {
            std::cerr << "Could not accept connection\n";
            exit(1);
        }


        std::shared_ptr<Resource> tPtr = std::make_shared<Resource>(directory);
        std::thread t1(manageConnection, connection, tPtr);
        t1.detach();

    }


    return 1;
}