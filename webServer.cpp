#include <bits/stdc++.h>
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <netinet/in.h>
#include <unistd.h>
#include "Resource/reqHeader.h"
#include "Resource/Resource.h"
#include "Resource/Utils.h"

int serviceRequest(int connection, Resource & resourceManager)
{
    ReqHeader reqInfo;
    int resource = 0;
    std::size_t size = 0; 
    
    if(reqInfo.getRequest(connection) < 0)
        return -1;

    if(reqInfo.status == 200)
    {   
        size = resourceManager.checkResource(reqInfo);
        if(size == 0)
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
	if(resourceManager.returnResource(connection,resource))
        {
            std::cerr << "Could not return resource\n";
            exit(1);
        }
    }

    else
        resourceManager.returnErrorMsg(connection, reqInfo);

    if(resource > 0)
        if(close(resource) < 0)
        {
            std::cerr << "Error closing resource\n";
            exit(1);
        }

    return 0;
}


void manageConnection(int connection, int socketListener, Resource & resourceManager)
{
    if(close(socketListener) < 0)
    {
        std::cerr << "Error closing socket on thread\n";
        exit(1);
    }

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
    Resource resource(directory);

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
            std::cerr << "Could no accept connection\n";
            exit(1);
        }

        //TODO: Manage multithreading here
        //-----------------
        serviceRequest(connection, resource);

        //--------

        if(close(connection) < 0)
        {
            std::cerr << "Error closing socket\n";
            exit(1);
        }
    }


    return 1;
}
