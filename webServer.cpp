#include <bits/stdc++.h>
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Resource/reqHeader.h"
#include "Resource/Resource.h"
#include "Resource/Utils.h"

int serviceRequest(int connection, std::shared_ptr<Resource> & resourceManager)
{
    ReqHeader reqInfo;
    std::size_t size = 0; 
    
    if(reqInfo.getRequest(connection) < 0)
        return -1;

    if(reqInfo.status == 200)
    {   
        size = resourceManager->checkResource(reqInfo);
        if(size == 0)
    {
        {
            reqInfo.status = 404;
        }
    }

    if(reqInfo.type == FULL)
    {
        outputHttpHeaders(connection, reqInfo.status,size);
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

char * getLocalIp(std::string & hostName)
{
    hostent * hostEntry = nullptr;
    char * ipBuffer;
    hostEntry = gethostbyname(hostName.data());
    if(hostEntry == nullptr)
    {
        std::cerr << "Invalid host name\n";
        exit(1);
    }
    ipBuffer = inet_ntoa(*(in_addr*) hostEntry->h_addr_list[0]);

    return ipBuffer;
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

    char * ipAddr = getLocalIp(hostName);
    if((sockListener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Could no create sock listener\n";
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    //servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_addr.s_addr = inet_addr(ipAddr);//htonl(INADDR_ANY);
    servAddr.sin_port = htons(std::stoi(port));

    int yes = 1;
    if(setsockopt(sockListener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        std::cerr << "Error setting socket options\n";
        exit(1);
    }

    if(bind(sockListener, (sockaddr * ) &servAddr, sizeof(servAddr)) < 0)
    {
        std::cerr << "Could not bind socket address or port\n";
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
